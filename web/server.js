const express = require('express');
const session = require('express-session');
const fs = require('fs');
const path = require('path');

const app = express();
const PORT = 3000;
const ADMIN_CODE = 'BIBLIO_ADMIN_2024'; // secret code required to register a new admin
const OMDB_KEY   = 'YOUR_OMDB_KEY';    // free key at https://www.omdbapi.com/apikey.aspx

const BIBLIOTECA_PATH = path.join(__dirname, '..', 'biblioteca.json');
const CONTURI_PATH = path.join(__dirname, '..', 'conturi.json');

app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));
app.use(session({
  secret: 'biblioteca-secret-2024',
  resave: false,
  saveUninitialized: false,
  cookie: { secure: false, maxAge: 24 * 60 * 60 * 1000 }
}));

// ── date helpers ──────────────────────────────────────────────────────────────

function todayStr() {
  return new Date().toISOString().split('T')[0];
}

function returnDateStr() {
  const d = new Date();
  d.setDate(d.getDate() + 14);
  return d.toISOString().split('T')[0];
}

// Normalize old-format string imprumuturi to objects
function normImp(imp) {
  if (typeof imp === 'string') {
    return { titlu: imp, dataImprumut: todayStr(), dataReturnare: returnDateStr() };
  }
  return imp;
}

// ── file helpers ──────────────────────────────────────────────────────────────

function readBiblioteca() {
  return JSON.parse(fs.readFileSync(BIBLIOTECA_PATH, 'utf8')).carti;
}

function writeBiblioteca(carti) {
  fs.writeFileSync(BIBLIOTECA_PATH, JSON.stringify({ carti }, null, 2));
}

function readConturi() {
  const raw = JSON.parse(fs.readFileSync(CONTURI_PATH, 'utf8')).conturi;
  return raw.map(c => ({ ...c, imprumuturi: (c.imprumuturi || []).map(normImp) }));
}

function writeConturi(conturi) {
  fs.writeFileSync(CONTURI_PATH, JSON.stringify({ conturi }, null, 2));
}

// ── middleware ────────────────────────────────────────────────────────────────

function requireAuth(req, res, next) {
  if (!req.session.user) return res.status(401).json({ error: 'Neautentificat' });
  next();
}

function requireAdmin(req, res, next) {
  if (!req.session.user) return res.status(401).json({ error: 'Neautentificat' });
  if (!req.session.user.esteAdmin) return res.status(403).json({ error: 'Acces interzis' });
  next();
}

function requireDirector(req, res, next) {
  if (!req.session.user) return res.status(401).json({ error: 'Neautentificat' });
  if (!req.session.user.esteAdmin || req.session.user.functie !== 'Director')
    return res.status(403).json({ error: 'Acces interzis — necesita rol Director' });
  next();
}

function safeUser(c) {
  return {
    username: c.username,
    nume: c.nume,
    prenume: c.prenume,
    dataNasterii: c.dataNasterii,
    esteAdmin: c.esteAdmin,
    email: c.email || '',
    telefon: c.telefon || '',
    functie: c.functie || '',
    imprumuturi: c.imprumuturi,
    favorite: c.favorite || [],
    salariu: c.salariu ?? null,
  };
}

// ── auth ──────────────────────────────────────────────────────────────────────

app.post('/api/login', (req, res) => {
  const { username, parola } = req.body;
  const conturi = readConturi();
  const cont = conturi.find(c => c.username === username && c.parola === parola);
  if (!cont) return res.status(401).json({ error: 'Username sau parola incorecta' });
  req.session.user = cont;
  res.json(safeUser(cont));
});

app.post('/api/logout', (req, res) => {
  req.session.destroy();
  res.json({ ok: true });
});

app.post('/api/register', (req, res) => {
  const { username, parola, nume, prenume, dataNasterii } = req.body;
  if (!username || !parola || !nume || !prenume) {
    return res.status(400).json({ error: 'Toate campurile sunt obligatorii' });
  }
  const conturi = readConturi();
  if (conturi.find(c => c.username === username)) {
    return res.status(400).json({ error: 'Username deja folosit' });
  }
  const cont = { username, parola, esteAdmin: false, nume, prenume, dataNasterii: dataNasterii || '', imprumuturi: [] };
  conturi.push(cont);
  writeConturi(conturi);
  req.session.user = cont;
  res.json(safeUser(cont));
});

app.post('/api/register-admin', (req, res) => {
  const { username, parola, nume, prenume, dataNasterii, email, telefon, functie, cod } = req.body;
  if (!username || !parola || !nume || !prenume) {
    return res.status(400).json({ error: 'Prenume, nume, username si parola sunt obligatorii' });
  }
  if (cod !== ADMIN_CODE) {
    return res.status(403).json({ error: 'Cod de acces incorect' });
  }
  const conturi = readConturi();
  if (conturi.find(c => c.username === username)) {
    return res.status(400).json({ error: 'Username deja folosit' });
  }
  const cont = {
    username, parola, esteAdmin: true,
    nume, prenume,
    dataNasterii: dataNasterii || '',
    email: email || '',
    telefon: telefon || '',
    functie: functie || 'Bibliotecar',
    imprumuturi: [],
  };
  conturi.push(cont);
  writeConturi(conturi);
  req.session.user = cont;
  res.json(safeUser(cont));
});

app.get('/api/me', requireAuth, (req, res) => {
  const conturi = readConturi();
  const cont = conturi.find(c => c.username === req.session.user.username);
  if (!cont) return res.status(404).json({ error: 'Cont negasit' });
  req.session.user = cont;
  res.json(safeUser(cont));
});

// ── carti ─────────────────────────────────────────────────────────────────────

app.get('/api/carti', (req, res) => {
  let carti = readBiblioteca();
  const { tip, q } = req.query;
  if (tip && tip !== 'all') carti = carti.filter(c => c.tip === tip);
  if (q) {
    const lower = q.toLowerCase();
    carti = carti.filter(c =>
      c.titlu.toLowerCase().includes(lower) ||
      c.autori.some(a => a.toLowerCase().includes(lower)) ||
      (c.serie && c.serie.toLowerCase().includes(lower))
    );
  }

  const conturi = readConturi();
  const borrowedMap = {};
  conturi.forEach(c => c.imprumuturi.forEach(i => {
    const titlu = typeof i === 'string' ? i : i.titlu;
    borrowedMap[titlu] = (borrowedMap[titlu] || 0) + 1;
  }));
  carti = carti.map(c => ({ ...c, copiiDisponibile: Math.max(0, (c.copii || 1) - (borrowedMap[c.titlu] || 0)) }));

  res.json({ carti });
});

app.post('/api/carti', requireAdmin, (req, res) => {
  const carti = readBiblioteca();
  const carte = req.body;
  if (!carte.titlu || !carte.tip) return res.status(400).json({ error: 'Titlu si tip sunt obligatorii' });
  if (carti.find(c => c.titlu === carte.titlu)) {
    return res.status(400).json({ error: 'Exista deja o carte cu acest titlu' });
  }
  carti.push(carte);
  writeBiblioteca(carti);
  res.json({ ok: true });
});

app.put('/api/carti/:titlu', requireAdmin, (req, res) => {
  const carti = readBiblioteca();
  const titluVechi = decodeURIComponent(req.params.titlu);
  const idx = carti.findIndex(c => c.titlu === titluVechi);
  if (idx === -1) return res.status(404).json({ error: 'Cartea nu a fost gasita' });

  const titluNou = req.body.titlu;
  if (titluNou !== titluVechi && carti.find(c => c.titlu === titluNou)) {
    return res.status(400).json({ error: 'Exista deja o carte cu titlul nou' });
  }

  // if title changed, update borrowed records and favorites too
  if (titluNou && titluNou !== titluVechi) {
    const conturi = readConturi();
    let changed = false;
    conturi.forEach(c => {
      c.imprumuturi.forEach(imp => {
        if (imp.titlu === titluVechi) { imp.titlu = titluNou; changed = true; }
      });
      if (c.favorite) {
        const fi = c.favorite.indexOf(titluVechi);
        if (fi !== -1) { c.favorite[fi] = titluNou; changed = true; }
      }
    });
    if (changed) writeConturi(conturi);
  }

  const prevRecenzii = carti[idx].recenzii;
  carti[idx] = req.body;
  if (prevRecenzii) carti[idx].recenzii = prevRecenzii;
  writeBiblioteca(carti);
  res.json({ ok: true });
});

app.delete('/api/carti/:titlu', requireAdmin, (req, res) => {
  let carti = readBiblioteca();
  const titlu = decodeURIComponent(req.params.titlu);
  const before = carti.length;
  carti = carti.filter(c => c.titlu !== titlu);
  if (carti.length === before) return res.status(404).json({ error: 'Cartea nu a fost gasita' });
  writeBiblioteca(carti);

  const conturi = readConturi();
  let changed = false;
  conturi.forEach(c => {
    if (c.favorite) {
      const prev = c.favorite.length;
      c.favorite = c.favorite.filter(t => t !== titlu);
      if (c.favorite.length !== prev) changed = true;
    }
  });
  if (changed) writeConturi(conturi);

  res.json({ ok: true });
});

// ── imprumuturi ───────────────────────────────────────────────────────────────

app.post('/api/imprumut', requireAuth, (req, res) => {
  const { titlu } = req.body;
  const carti = readBiblioteca();
  const carte = carti.find(c => c.titlu === titlu);
  const conturi = readConturi();

  if (carte?.copii) {
    const borrowedCount = conturi.filter(c => c.imprumuturi.some(i => (typeof i === 'string' ? i : i.titlu) === titlu)).length;
    if (borrowedCount >= carte.copii) return res.status(400).json({ error: 'Nu mai sunt copii disponibile' });
  }

  const idx = conturi.findIndex(c => c.username === req.session.user.username);
  if (idx === -1) return res.status(404).json({ error: 'Cont negasit' });
  if (conturi[idx].imprumuturi.find(i => i.titlu === titlu)) {
    return res.status(400).json({ error: 'Ai deja aceasta carte imprumutata' });
  }
  const entry = { titlu, dataImprumut: todayStr(), dataReturnare: returnDateStr() };
  conturi[idx].imprumuturi.push(entry);
  writeConturi(conturi);
  req.session.user = conturi[idx];
  res.json({ ok: true, imprumuturi: conturi[idx].imprumuturi });
});

app.delete('/api/imprumut/:titlu', requireAuth, (req, res) => {
  const titlu = decodeURIComponent(req.params.titlu);
  const conturi = readConturi();
  const idx = conturi.findIndex(c => c.username === req.session.user.username);
  if (idx === -1) return res.status(404).json({ error: 'Cont negasit' });
  const impIdx = conturi[idx].imprumuturi.findIndex(i => i.titlu === titlu);
  if (impIdx === -1) return res.status(400).json({ error: 'Nu ai aceasta carte imprumutata' });
  conturi[idx].imprumuturi.splice(impIdx, 1);
  writeConturi(conturi);
  req.session.user = conturi[idx];
  res.json({ ok: true, imprumuturi: conturi[idx].imprumuturi });
});

// ── users (admin) ─────────────────────────────────────────────────────────────

app.get('/api/users', requireAdmin, (req, res) => {
  const conturi = readConturi();
  res.json({ conturi: conturi.map(safeUser) });
});

app.put('/api/users/:username', requireDirector, (req, res) => {
  const username = req.params.username;
  const conturi = readConturi();
  const idx = conturi.findIndex(c => c.username === username);
  if (idx === -1) return res.status(404).json({ error: 'Utilizatorul nu a fost gasit' });

  const { prenume, nume, dataNasterii, parola, email, telefon, functie, salariu } = req.body;
  if (prenume) conturi[idx].prenume = prenume;
  if (nume) conturi[idx].nume = nume;
  if (dataNasterii !== undefined) conturi[idx].dataNasterii = dataNasterii;
  if (parola && parola.trim()) conturi[idx].parola = parola.trim();
  if (email !== undefined) conturi[idx].email = email;
  if (telefon !== undefined) conturi[idx].telefon = telefon;
  if (functie) conturi[idx].functie = functie;
  if (salariu !== undefined) conturi[idx].salariu = salariu;

  writeConturi(conturi);
  res.json({ ok: true, user: safeUser(conturi[idx]) });
});

app.delete('/api/users/:username', requireDirector, (req, res) => {
  const username = req.params.username;
  if (username === req.session.user.username) {
    return res.status(400).json({ error: 'Nu te poti sterge pe tine insuti' });
  }
  let conturi = readConturi();
  const before = conturi.length;
  conturi = conturi.filter(c => c.username !== username);
  if (conturi.length === before) return res.status(404).json({ error: 'Utilizatorul nu a fost gasit' });
  writeConturi(conturi);
  res.json({ ok: true });
});

// ── Admin: fetch covers ───────────────────────────────────────────────────────

async function fetchWithTimeout(url, ms = 5000) {
  const ctrl = new AbortController();
  const timer = setTimeout(() => ctrl.abort(), ms);
  try { return await fetch(url, { signal: ctrl.signal }); }
  finally { clearTimeout(timer); }
}

async function fetchCoverForBook(carte) {
  if (carte.imagine && carte.imagine.trim()) return false;
  try {
    let url = null;

    if (carte.tip === 'FilmCarte' && OMDB_KEY && OMDB_KEY !== 'YOUR_OMDB_KEY') {
      try {
        const r = await fetchWithTimeout(`https://www.omdbapi.com/?t=${encodeURIComponent(carte.titlu)}&type=movie&apikey=${OMDB_KEY}`);
        const d = await r.json();
        if (d.Response === 'True' && d.Poster && d.Poster !== 'N/A') url = d.Poster;
      } catch {}
    }

    if (!url) {
      const r = await fetchWithTimeout(`https://openlibrary.org/search.json?q=${encodeURIComponent(carte.titlu)}&fields=cover_i&limit=1`);
      const d = await r.json();
      const doc = (d.docs || []).find(x => x.cover_i);
      if (doc?.cover_i) url = `https://covers.openlibrary.org/b/id/${doc.cover_i}-M.jpg`;
    }

    if (url) { carte.imagine = url; return true; }
  } catch {}
  return false;
}

app.post('/api/admin/fetch-covers', requireAdmin, async (req, res) => {
  const carti = readBiblioteca();
  const toProcess = carti.filter(c => !c.imagine || !c.imagine.trim());
  const skipped = carti.length - toProcess.length;
  let found = 0;

  const BATCH = 25;
  for (let i = 0; i < toProcess.length; i += BATCH) {
    const results = await Promise.all(toProcess.slice(i, i + BATCH).map(fetchCoverForBook));
    found += results.filter(Boolean).length;
  }

  writeBiblioteca(carti);
  res.json({ ok: true, found, skipped, total: carti.length });
});

// ── API search ────────────────────────────────────────────────────────────────

app.get('/api/search/book', async (req, res) => {
  const q = (req.query.q || '').trim();
  if (!q) return res.json({ results: [] });
  try {
    const url = `https://openlibrary.org/search.json?q=${encodeURIComponent(q)}&fields=title,author_name,isbn,cover_i,first_publish_year,series,edition_count,subject&limit=8`;
    const resp = await fetch(url);
    const data = await resp.json();
    const results = (data.docs || []).map(doc => ({
      titlu:    doc.title || '',
      autori:   doc.author_name || [],
      isbn:     (doc.isbn || [])[0] || '',
      serie:    (doc.series || [])[0] || '',
      imagine:  doc.cover_i ? `https://covers.openlibrary.org/b/id/${doc.cover_i}-M.jpg` : '',
      an:       doc.first_publish_year || '',
      editie:   doc.edition_count || 1,
      detalii:  '',
    }));
    res.json({ results });
  } catch (err) {
    console.error('Open Library error:', err.message);
    res.status(500).json({ error: 'Eroare la cautarea cartilor (Open Library)' });
  }
});

app.get('/api/search/film', async (req, res) => {
  const q = (req.query.q || '').trim();
  if (!q) return res.json({ results: [] });
  if (!OMDB_KEY || OMDB_KEY === 'YOUR_OMDB_KEY') {
    return res.status(503).json({ error: 'OMDB_KEY_MISSING' });
  }
  try {
    const searchUrl = `https://www.omdbapi.com/?s=${encodeURIComponent(q)}&type=movie&apikey=${OMDB_KEY}`;
    const searchResp = await fetch(searchUrl);
    const searchData = await searchResp.json();
    if (searchData.Response === 'False') return res.json({ results: [] });

    const top = (searchData.Search || []).slice(0, 6);
    const results = await Promise.all(top.map(async item => {
      try {
        const dResp = await fetch(`https://www.omdbapi.com/?i=${item.imdbID}&apikey=${OMDB_KEY}`);
        const d = await dResp.json();
        return {
          titlu:     d.Title || item.Title || '',
          autori:    d.Writer && d.Writer !== 'N/A' ? d.Writer.split(/, ?/) : [],
          regizor:   d.Director !== 'N/A' ? d.Director : '',
          anLansare: parseInt(d.Year) || parseInt(item.Year) || 0,
          imagine:   d.Poster && d.Poster !== 'N/A' ? d.Poster : '',
          rating:    d.imdbRating && d.imdbRating !== 'N/A' ? Math.min(10, parseFloat(d.imdbRating)) : 0,
          detalii:   d.Plot && d.Plot !== 'N/A' ? d.Plot : '',
          isbn:      '',
        };
      } catch {
        return { titlu: item.Title, autori: [], regizor: '', anLansare: parseInt(item.Year) || 0, imagine: item.Poster !== 'N/A' ? item.Poster : '', rating: 0, detalii: '', isbn: '' };
      }
    }));
    res.json({ results });
  } catch (err) {
    console.error('OMDB error:', err.message);
    res.status(500).json({ error: 'Eroare la cautarea filmelor (OMDB)' });
  }
});

// ── amenzi ───────────────────────────────────────────────────────────────────

app.post('/api/amenzi/stinge', requireAdmin, (req, res) => {
  const { username, titlu } = req.body;
  const conturi = readConturi();
  const idx = conturi.findIndex(c => c.username === username);
  if (idx === -1) return res.status(404).json({ error: 'Utilizatorul nu a fost gasit' });
  const imp = conturi[idx].imprumuturi.find(i => i.titlu === titlu);
  if (!imp) return res.status(404).json({ error: 'Imprumut negasit' });
  imp.amendaStinsa = true;
  writeConturi(conturi);
  res.json({ ok: true });
});

// ── recenzii ─────────────────────────────────────────────────────────────────

app.post('/api/recenzii/:titlu', requireAuth, (req, res) => {
  const titlu = decodeURIComponent(req.params.titlu);
  const { rating, comment } = req.body;
  if (!rating || rating < 1 || rating > 5) return res.status(400).json({ error: 'Rating invalid (1-5)' });
  const carti = readBiblioteca();
  const carte = carti.find(c => c.titlu === titlu);
  if (!carte) return res.status(404).json({ error: 'Cartea nu a fost gasita' });
  if (!carte.recenzii) carte.recenzii = [];
  const rec = { username: req.session.user.username, rating: parseInt(rating), comment: (comment || '').trim().slice(0, 500), data: todayStr() };
  const existingIdx = carte.recenzii.findIndex(r => r.username === rec.username);
  if (existingIdx !== -1) carte.recenzii[existingIdx] = rec;
  else carte.recenzii.push(rec);
  writeBiblioteca(carti);
  res.json({ ok: true, recenzii: carte.recenzii });
});

app.delete('/api/recenzii/:titlu', requireAuth, (req, res) => {
  const titlu = decodeURIComponent(req.params.titlu);
  const carti = readBiblioteca();
  const carte = carti.find(c => c.titlu === titlu);
  if (!carte) return res.status(404).json({ error: 'Cartea nu a fost gasita' });
  carte.recenzii = (carte.recenzii || []).filter(r => r.username !== req.session.user.username);
  writeBiblioteca(carti);
  res.json({ ok: true, recenzii: carte.recenzii });
});

// ── favorite ──────────────────────────────────────────────────────────────────

app.post('/api/favorite', requireAuth, (req, res) => {
  const { titlu } = req.body;
  if (!titlu) return res.status(400).json({ error: 'Titlu lipsa' });
  const conturi = readConturi();
  const idx = conturi.findIndex(c => c.username === req.session.user.username);
  if (idx === -1) return res.status(404).json({ error: 'Cont negasit' });
  if (!conturi[idx].favorite) conturi[idx].favorite = [];
  if (!conturi[idx].favorite.includes(titlu)) conturi[idx].favorite.push(titlu);
  writeConturi(conturi);
  req.session.user = conturi[idx];
  res.json({ ok: true, favorite: conturi[idx].favorite });
});

app.delete('/api/favorite/:titlu', requireAuth, (req, res) => {
  const titlu = decodeURIComponent(req.params.titlu);
  const conturi = readConturi();
  const idx = conturi.findIndex(c => c.username === req.session.user.username);
  if (idx === -1) return res.status(404).json({ error: 'Cont negasit' });
  conturi[idx].favorite = (conturi[idx].favorite || []).filter(t => t !== titlu);
  writeConturi(conturi);
  req.session.user = conturi[idx];
  res.json({ ok: true, favorite: conturi[idx].favorite });
});

app.listen(PORT, () => {
  console.log(`\n  Biblioteca ruleaza la http://localhost:${PORT}\n`);
});
