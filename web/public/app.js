// ── State ─────────────────────────────────────────────────────────────────────
const state = {
  user: null,
  carti: [],
  filter: 'all',
  search: '',
  view: 'library',
  sort: 'default',
  excludeTypes: [],
  minRating: 0,
};

// ── Translations ─────────────────────────────────────────────────────────────
const TRANSLATIONS = {
  ro: {
    tab_login:'Login', tab_register:'Cont nou', btn_connect:'Conecteaza-te',
    btn_create_account:'Creeaza cont', btn_create_admin:'Creeaza cont Admin',
    btn_guest:'Continua fara cont →', admin_reg_banner:'🔑 Cont Administrator',
    nav_catalog:'Catalog', nav_mybooks:'Cartile mele', nav_admin:'Admin',
    btn_logout:'Logout', btn_login:'Login',
    hero_title:'Bun venit în Bibliotecă',
    hero_sub:'200 de titluri — carti digitale, audiobook-uri, manga si filme',
    search_ph:'Cauta dupa titlu, autor sau serie...',
    filter_all:'Toate', filter_digital:'Digital', filter_audio:'Audiobook',
    filter_manga:'Manga', filter_film:'Film', filter_fav:'Favorite',
    sort_default:'Sortare: Implicita', sort_az:'A → Z', sort_za:'Z → A',
    sort_rating_desc:'★ Rating descrescator', sort_rating_asc:'★ Rating crescator',
    sort_newest:'An: Nou → Vechi', sort_oldest:'An: Vechi → Nou',
    btn_adv_filter:'⚙ Filtru avansat', btn_reset_filters:'✕ Reset filtre',
    lbl_exclude:'Exclude tipuri', lbl_min_rating:'Rating minim',
    no_results:'Niciun rezultat gasit.', btn_go_catalog:'Mergi la catalog',
    mybooks_empty:'Nu ai nicio carte imprumutata.',
    borrow_btn:'+ Imprumuta', borrowed_btn:'✓ Imprumutat',
    login_to_borrow:'Login pentru imprumut', return_btn:'↩ Returneaza',
    admin_panel:'Panou Admin', add_book_title:'Adauga carte',
    users_title:'Utilizatori', all_books_title:'Toate cartile',
    btn_fetch_covers:'🖼 Fetch Covers', tab_api:'🔍 Cauta cu API', tab_manual:'✏️ Manual',
    lbl_username:'Username', lbl_parola:'Parola', lbl_prenume:'Prenume', lbl_nume:'Nume',
    lbl_data:'Data nasterii', lbl_telefon:'Telefon', lbl_email:'Email',
    lbl_functie:'Functie', lbl_access_code:'Cod de acces admin',
    ph_username:'username', ph_prenume:'Ion', ph_nume:'Popescu',
    ph_telefon:'07xx xxx xxx', ph_email:'email@biblioteca.ro',
    ph_access_code:'Codul secret de inregistrare',
    hint_access_code:'Cereti codul de la un admin existent.',
  },
  en: {
    tab_login:'Login', tab_register:'New account', btn_connect:'Sign in',
    btn_create_account:'Create account', btn_create_admin:'Create Admin account',
    btn_guest:'Continue without account →', admin_reg_banner:'🔑 Administrator Account',
    nav_catalog:'Catalog', nav_mybooks:'My Books', nav_admin:'Admin',
    btn_logout:'Logout', btn_login:'Login',
    hero_title:'Welcome to the Library',
    hero_sub:'200 titles — digital books, audiobooks, manga and films',
    search_ph:'Search by title, author or series...',
    filter_all:'All', filter_digital:'Digital', filter_audio:'Audiobook',
    filter_manga:'Manga', filter_film:'Film', filter_fav:'Favorites',
    sort_default:'Sort: Default', sort_az:'A → Z', sort_za:'Z → A',
    sort_rating_desc:'★ Rating descending', sort_rating_asc:'★ Rating ascending',
    sort_newest:'Year: New → Old', sort_oldest:'Year: Old → New',
    btn_adv_filter:'⚙ Advanced filter', btn_reset_filters:'✕ Reset filters',
    lbl_exclude:'Exclude types', lbl_min_rating:'Minimum rating',
    no_results:'No results found.', btn_go_catalog:'Go to catalog',
    mybooks_empty:'You have no borrowed books.',
    borrow_btn:'+ Borrow', borrowed_btn:'✓ Borrowed',
    login_to_borrow:'Login to borrow', return_btn:'↩ Return',
    admin_panel:'Admin Panel', add_book_title:'Add book',
    users_title:'Users', all_books_title:'All books',
    btn_fetch_covers:'🖼 Fetch Covers', tab_api:'🔍 Search with API', tab_manual:'✏️ Manual',
    lbl_username:'Username', lbl_parola:'Password', lbl_prenume:'First name', lbl_nume:'Last name',
    lbl_data:'Date of birth', lbl_telefon:'Phone', lbl_email:'Email',
    lbl_functie:'Role', lbl_access_code:'Admin access code',
    ph_username:'username', ph_prenume:'John', ph_nume:'Smith',
    ph_telefon:'+40 7xx xxx xxx', ph_email:'email@library.com',
    ph_access_code:'Secret registration code',
    hint_access_code:'Ask an existing admin for the code.',
  },
  uk: {
    tab_login:'Вхід', tab_register:'Новий акаунт', btn_connect:'Увійти',
    btn_create_account:'Створити акаунт', btn_create_admin:'Створити акаунт адміна',
    btn_guest:'Продовжити без акаунту →', admin_reg_banner:'🔑 Акаунт адміністратора',
    nav_catalog:'Каталог', nav_mybooks:'Мої книги', nav_admin:'Адмін',
    btn_logout:'Вийти', btn_login:'Увійти',
    hero_title:'Ласкаво просимо до Бібліотеки',
    hero_sub:'200 назв — електронні книги, аудіокниги, манга та фільми',
    search_ph:'Пошук за назвою, автором або серією...',
    filter_all:'Усі', filter_digital:'Цифрові', filter_audio:'Аудіокниги',
    filter_manga:'Манга', filter_film:'Фільми', filter_fav:'Вибрані',
    sort_default:'Сортування: За замовчуванням', sort_az:'А → Я', sort_za:'Я → А',
    sort_rating_desc:'★ Рейтинг: спадання', sort_rating_asc:'★ Рейтинг: зростання',
    sort_newest:'Рік: Нові → Старі', sort_oldest:'Рік: Старі → Нові',
    btn_adv_filter:'⚙ Розширений фільтр', btn_reset_filters:'✕ Скинути фільтри',
    lbl_exclude:'Виключити типи', lbl_min_rating:'Мінімальний рейтинг',
    no_results:'Нічого не знайдено.', btn_go_catalog:'До каталогу',
    mybooks_empty:'У вас немає позичених книг.',
    borrow_btn:'+ Позичити', borrowed_btn:'✓ Позичено',
    login_to_borrow:'Увійдіть для позичання', return_btn:'↩ Повернути',
    admin_panel:'Панель адміна', add_book_title:'Додати книгу',
    users_title:'Користувачі', all_books_title:'Усі книги',
    btn_fetch_covers:'🖼 Завантажити обкладинки', tab_api:'🔍 Пошук через API', tab_manual:'✏️ Вручну',
    lbl_username:'Логін', lbl_parola:'Пароль', lbl_prenume:"Ім'я", lbl_nume:'Прізвище',
    lbl_data:'Дата народження', lbl_telefon:'Телефон', lbl_email:'Email',
    lbl_functie:'Посада', lbl_access_code:'Код доступу адміна',
    ph_username:'логін', ph_prenume:'Іван', ph_nume:'Петренко',
    ph_telefon:'+380 xx xxx xxxx', ph_email:'email@бібліотека.ua',
    ph_access_code:'Секретний код реєстрації',
    hint_access_code:'Запитайте код у існуючого адміна.',
  },
};

let currentLang  = localStorage.getItem('lang')  || 'ro';
let currentTheme = localStorage.getItem('theme') || 'dark';

function t(key) { return (TRANSLATIONS[currentLang] || TRANSLATIONS.ro)[key] || key; }

function applyLang() {
  document.documentElement.lang = currentLang;
  document.querySelectorAll('[data-i18n]').forEach(el => { el.textContent = t(el.dataset.i18n); });
  document.querySelectorAll('[data-i18n-ph]').forEach(el => { el.placeholder = t(el.dataset.i18nPh); });
  document.querySelectorAll('[data-i18n-opt]').forEach(el => { el.textContent = t(el.dataset.i18nOpt); });
  document.querySelectorAll('.lang-btn').forEach(btn => btn.classList.toggle('active', btn.dataset.lang === currentLang));
}

function setLang(lang) {
  currentLang = lang;
  localStorage.setItem('lang', lang);
  applyLang();
  renderBooks();
  if (state.view === 'mybooks') renderMyBooks();
}

function toggleTheme() {
  currentTheme = currentTheme === 'dark' ? 'light' : 'dark';
  localStorage.setItem('theme', currentTheme);
  applyTheme();
}

function applyTheme() {
  document.documentElement.setAttribute('data-theme', currentTheme);
  document.getElementById('theme-toggle').textContent = currentTheme === 'dark' ? '🌙' : '☀️';
}

// ── API ───────────────────────────────────────────────────────────────────────
async function api(method, path, body) {
  const opts = { method, headers: { 'Content-Type': 'application/json' } };
  if (body) opts.body = JSON.stringify(body);
  const res = await fetch(path, opts);
  const data = await res.json().catch(() => ({}));
  if (!res.ok) throw new Error(data.error || 'Eroare server');
  return data;
}

// ── Toast ─────────────────────────────────────────────────────────────────────
let toastTimer;
function toast(msg, type = '') {
  const el = document.getElementById('toast');
  el.textContent = msg;
  el.className = type ? `show ${type}` : 'show';
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => { el.className = ''; }, 3200);
}

// ── Date helpers ──────────────────────────────────────────────────────────────
function fmtDate(iso) {
  if (!iso) return '—';
  const [y, m, d] = iso.split('-');
  return `${d}.${m}.${y}`;
}

function daysUntil(iso) {
  const now = new Date(); now.setHours(0, 0, 0, 0);
  return Math.round((new Date(iso) - now) / 86400000);
}

function calcAmenda(imp) {
  if (!imp || imp.amendaStinsa) return 0;
  const days = -daysUntil(imp.dataReturnare);
  return days > 0 ? parseFloat((days * 0.5).toFixed(2)) : 0;
}

function dateStatusHtml(imp) {
  const days = daysUntil(imp.dataReturnare);
  if (days < 0) return `<span class="date-status date-overdue">⚠ Intarziat cu ${Math.abs(days)} zile</span>`;
  if (days <= 3) return `<span class="date-status date-warn">⏳ ${days} zile ramase</span>`;
  return `<span class="date-status date-ok">✓ ${days} zile ramase</span>`;
}

// ── Role helpers ──────────────────────────────────────────────────────────────
function isDirector() {
  return state.user?.esteAdmin && state.user?.functie === 'Director';
}

// ── Favorite helpers ──────────────────────────────────────────────────────────
function getFavorites() {
  return state.user?.favorite || [];
}

async function toggleFavorite(titlu) {
  if (!state.user) { toast('Trebuie sa fii autentificat', 'error'); return; }
  const isFav = getFavorites().includes(titlu);
  try {
    const r = isFav
      ? await api('DELETE', `/api/favorite/${encodeURIComponent(titlu)}`)
      : await api('POST', '/api/favorite', { titlu });
    state.user.favorite = r.favorite;
    _recoKey = ''; // invalidate reco cache
    updateCounts();
    renderBooks();
    renderRecommendations();
  } catch (err) { toast(err.message, 'error'); }
}

// ── Borrow helpers ────────────────────────────────────────────────────────────
function getBorrowedTitles() {
  return (state.user?.imprumuturi || []).map(i => typeof i === 'string' ? i : i.titlu);
}

function findBorrowed(titlu) {
  return (state.user?.imprumuturi || []).find(i =>
    (typeof i === 'string' ? i : i.titlu) === titlu
  );
}

// ── Session ───────────────────────────────────────────────────────────────────
async function tryRestoreSession() {
  try {
    state.user = await api('GET', '/api/me');
    enterApp();
  } catch {
    showAuthOverlay();
  }
}

function showAuthOverlay() {
  document.getElementById('auth-overlay').classList.remove('hidden');
  document.getElementById('app').classList.add('hidden');
}

function enterApp() {
  document.getElementById('auth-overlay').classList.add('hidden');
  document.getElementById('app').classList.remove('hidden');
  applyLang();
  applyTheme();

  document.getElementById('nav-username').textContent = state.user
    ? `@${state.user.username}${state.user.esteAdmin ? ' · ' + (state.user.functie || 'Admin') : ''}`
    : '👤 Oaspete';
  document.getElementById('logout-btn').classList.toggle('hidden', !state.user);
  document.getElementById('login-shortcut').classList.toggle('hidden', !!state.user);
  document.getElementById('nav-mybooks').classList.toggle('hidden', !state.user);
  document.querySelectorAll('.admin-only').forEach(b =>
    b.classList.toggle('hidden', !state.user?.esteAdmin)
  );

  showView('library');
  loadCarti();
}

// ── Auth events ───────────────────────────────────────────────────────────────
document.getElementById('login-form').addEventListener('submit', async e => {
  e.preventDefault();
  try {
    state.user = await api('POST', '/api/login', {
      username: document.getElementById('login-username').value.trim(),
      parola: document.getElementById('login-parola').value,
    });
    toast(`Bun venit, ${state.user.prenume}!`, 'success');
    enterApp();
  } catch (err) { toast(err.message, 'error'); }
});

document.getElementById('register-form').addEventListener('submit', async e => {
  e.preventDefault();
  try {
    state.user = await api('POST', '/api/register', {
      username: document.getElementById('reg-username').value.trim(),
      parola: document.getElementById('reg-parola').value,
      prenume: document.getElementById('reg-prenume').value.trim(),
      nume: document.getElementById('reg-nume').value.trim(),
      dataNasterii: document.getElementById('reg-data').value.trim(),
    });
    toast(`Cont creat! Bun venit, ${state.user.prenume}!`, 'success');
    enterApp();
  } catch (err) { toast(err.message, 'error'); }
});

document.getElementById('logout-btn').addEventListener('click', async () => {
  await api('POST', '/api/logout');
  state.user = null;
  toast('La revedere!');
  showAuthOverlay();
});

document.getElementById('browse-guest').addEventListener('click', () => {
  state.user = null;
  enterApp();
});

document.getElementById('login-shortcut').addEventListener('click', () => {
  document.getElementById('app').classList.add('hidden');
  document.getElementById('auth-overlay').classList.remove('hidden');
});

document.getElementById('register-admin-form').addEventListener('submit', async e => {
  e.preventDefault();
  try {
    state.user = await api('POST', '/api/register-admin', {
      username:    document.getElementById('adm-username').value.trim(),
      parola:      document.getElementById('adm-parola').value,
      prenume:     document.getElementById('adm-prenume').value.trim(),
      nume:        document.getElementById('adm-nume').value.trim(),
      dataNasterii:document.getElementById('adm-data').value.trim(),
      email:       document.getElementById('adm-email').value.trim(),
      telefon:     document.getElementById('adm-telefon').value.trim(),
      functie:     document.getElementById('adm-functie').value,
      cod:         document.getElementById('adm-cod').value,
    });
    toast(`Cont admin creat! Bun venit, ${state.user.prenume}!`, 'success');
    enterApp();
  } catch (err) { toast(err.message, 'error'); }
});

// apply lang+theme on auth overlay too so buttons show correctly before login
applyLang();
applyTheme();

document.querySelectorAll('.lang-btn').forEach(btn => {
  btn.addEventListener('click', () => setLang(btn.dataset.lang));
});

document.querySelectorAll('.auth-tab').forEach(btn => {
  btn.addEventListener('click', () => {
    document.querySelectorAll('.auth-tab').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    const tab = btn.dataset.tab;
    document.getElementById('login-form').classList.toggle('hidden', tab !== 'login');
    document.getElementById('register-form').classList.toggle('hidden', tab !== 'register');
    document.getElementById('register-admin-form').classList.toggle('hidden', tab !== 'register-admin');
  });
});

// ── Navigation ────────────────────────────────────────────────────────────────
function showView(view) {
  state.view = view;
  document.querySelectorAll('[id^="view-"]').forEach(el => el.classList.add('hidden'));
  document.getElementById(`view-${view}`).classList.remove('hidden');
  document.querySelectorAll('.nav-btn').forEach(b =>
    b.classList.toggle('active', b.dataset.view === view)
  );
  if (view === 'mybooks') renderMyBooks();
  if (view === 'admin') renderAdmin();
}

document.querySelectorAll('.nav-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    if (!state.user && btn.dataset.view !== 'library') {
      toast('Trebuie sa fii autentificat', 'error'); return;
    }
    showView(btn.dataset.view);
  });
});

// ── Load books ────────────────────────────────────────────────────────────────
async function loadCarti() {
  try {
    const { carti } = await api('GET', '/api/carti');
    state.carti = carti;
    updateCounts();
    renderBooks();
    renderRecommendations();
  } catch { toast('Eroare la incarcarea cartilor', 'error'); }
}

function updateCounts() {
  document.getElementById('count-all').textContent = state.carti.length;
  ['CarteDigitala', 'Audiobook', 'MangaComics', 'FilmCarte'].forEach(t => {
    const el = document.getElementById(`count-${t}`);
    if (el) el.textContent = state.carti.filter(c => c.tip === t).length;
  });
  const favEl = document.getElementById('count-favorites');
  if (favEl) favEl.textContent = getFavorites().length;
}

function filteredBooks() {
  let list = state.carti;

  if (state.filter === 'favorites') {
    const favs = getFavorites();
    list = list.filter(c => favs.includes(c.titlu));
  } else if (state.filter !== 'all') {
    list = list.filter(c => c.tip === state.filter);
  }

  if (state.excludeTypes.length)
    list = list.filter(c => !state.excludeTypes.includes(c.tip));

  if (state.minRating > 0)
    list = list.filter(c => c.rating >= state.minRating);

  if (state.search) {
    const q = state.search.toLowerCase();
    list = list.filter(c =>
      c.titlu.toLowerCase().includes(q) ||
      c.autori.some(a => a.toLowerCase().includes(q)) ||
      (c.serie && c.serie.toLowerCase().includes(q))
    );
  }

  switch (state.sort) {
    case 'az':          list = [...list].sort((a, b) => a.titlu.localeCompare(b.titlu)); break;
    case 'za':          list = [...list].sort((a, b) => b.titlu.localeCompare(a.titlu)); break;
    case 'rating-desc': list = [...list].sort((a, b) => b.rating - a.rating); break;
    case 'rating-asc':  list = [...list].sort((a, b) => a.rating - b.rating); break;
    case 'newest':      list = [...list].sort((a, b) => (b.anLansare || b.an || 0) - (a.anLansare || a.an || 0)); break;
    case 'oldest':      list = [...list].sort((a, b) => (a.anLansare || a.an || 0) - (b.anLansare || b.an || 0)); break;
  }

  return list;
}

// ── Sort & advanced filter controls ──────────────────────────────────────────
function toggleAdvFilters() {
  const panel = document.getElementById('advanced-filters');
  const btn = document.getElementById('adv-filter-btn');
  const open = panel.classList.toggle('hidden') === false;
  btn.classList.toggle('adv-active', open);
}

function applySort(val) {
  state.sort = val;
  updateResetBtn();
  renderBooks();
}

function applyExclude() {
  state.excludeTypes = [...document.querySelectorAll('.adv-checks input:checked')].map(el => el.value);
  updateResetBtn();
  renderBooks();
}

function applyRating(val) {
  state.minRating = parseFloat(val);
  document.getElementById('rating-val').textContent = parseFloat(val).toFixed(1);
  updateResetBtn();
  renderBooks();
}

function updateResetBtn() {
  const active = state.sort !== 'default' || state.excludeTypes.length > 0 || state.minRating > 0;
  document.getElementById('reset-adv-btn').classList.toggle('hidden', !active);
  document.getElementById('adv-filter-btn').classList.toggle('adv-active',
    !document.getElementById('advanced-filters').classList.contains('hidden'));
}

function resetAdvFilters() {
  state.sort = 'default';
  state.excludeTypes = [];
  state.minRating = 0;
  document.getElementById('sort-select').value = 'default';
  document.querySelectorAll('.adv-checks input').forEach(el => el.checked = false);
  document.getElementById('rating-range').value = 0;
  document.getElementById('rating-val').textContent = '0.0';
  updateResetBtn();
  renderBooks();
}

function renderBooks() {
  const books = filteredBooks();
  const grid = document.getElementById('books-grid');
  const empty = document.getElementById('empty-state');
  const info = document.getElementById('results-info');
  const borrowed = getBorrowedTitles();

  if (!books.length) {
    grid.innerHTML = '';
    empty.classList.remove('hidden');
    info.classList.add('hidden');
  } else {
    empty.classList.add('hidden');
    info.classList.toggle('hidden', !state.search);
    if (state.search) info.textContent = `${books.length} rezultat${books.length !== 1 ? 'e' : ''} pentru "${state.search}"`;
    grid.innerHTML = books.map((c, i) => bookCard(c, borrowed.includes(c.titlu), i)).join('');
  }
}

const TYPE_LABEL  = { CarteDigitala: '📱 Digital', Audiobook: '🎧 Audiobook', MangaComics: '📖 Manga', FilmCarte: '🎬 Film' };
const TYPE_EMOJI  = { CarteDigitala: '📱', Audiobook: '🎧', MangaComics: '📖', FilmCarte: '🎬' };

function coverUrl(c) {
  if (c.imagine && c.imagine.trim()) return c.imagine.trim();
  const isbn = (c.isbn || '').replace(/[-\s]/g, '');
  if (isbn) return `https://covers.openlibrary.org/b/isbn/${isbn}-M.jpg`;
  return null;
}

function coverImgHtml(c, cls = '') {
  const url = coverUrl(c);
  const letter = (c.titlu || '?')[0].toUpperCase();
  const ph = `<div class="cover-placeholder ph-${c.tip}"><span>${TYPE_EMOJI[c.tip]}</span><span class="cover-placeholder-letter">${esc(letter)}</span></div>`;
  if (!url) return ph;
  const phEsc = ph.replace(/"/g, '&quot;').replace(/'/g, '&#39;');
  return `<img src="${esc(url)}" class="${cls}" alt="${esc(c.titlu)}" onerror="this.outerHTML='${phEsc}'"/>`;
}

function cardExtra(c) {
  if (c.tip === 'FilmCarte') return `${c.anLansare} · ${c.regizor}`;
  if (c.tip === 'Audiobook') return `${c.narator} · ${Math.floor(c.durataMinute / 60)}h ${c.durataMinute % 60}m`;
  if (c.tip === 'MangaComics') return `${c.stilDesen}${c.esteColor ? ' · Color' : ' · A/N'}`;
  if (c.tip === 'CarteDigitala') return `${c.formatFisier}${c.areDrm ? ' · DRM' : ''}`;
  return '';
}

function bookCard(c, isBorrowed, idx) {
  const isFav = getFavorites().includes(c.titlu);
  const avail = c.copiiDisponibile ?? c.copii ?? 1;
  const outOfCopies = !isBorrowed && avail <= 0;

  let borrowBtn;
  if (!state.user)
    borrowBtn = `<button class="borrow-btn need-login" onclick="event.stopPropagation(); document.getElementById('login-shortcut').click()">${t('login_to_borrow')}</button>`;
  else if (isBorrowed)
    borrowBtn = `<button class="borrow-btn borrowed" disabled>${t('borrowed_btn')}</button>`;
  else if (outOfCopies)
    borrowBtn = `<button class="borrow-btn" disabled style="background:rgba(248,113,113,0.15);color:var(--danger);border:1px solid rgba(248,113,113,0.3);cursor:default">Epuizat</button>`;
  else
    borrowBtn = `<button class="borrow-btn can-borrow" onclick="event.stopPropagation(); borrowBook('${esc(c.titlu)}')">${t('borrow_btn')}</button>`;

  const heartBtn = state.user
    ? `<button class="card-heart" onclick="event.stopPropagation(); toggleFavorite('${esc(c.titlu)}')" title="${isFav ? 'Sterge din favorite' : 'Adauga la favorite'}">${isFav ? '❤️' : '🤍'}</button>`
    : '';

  const copiesClass = avail <= 0 ? 'avail-out' : avail <= 5 ? 'avail-low' : 'avail-good';
  const copiesBadge = c.copii != null
    ? `<span class="card-copies ${copiesClass}">📚 ${avail}</span>`
    : '';

  const extra = cardExtra(c);
  return `
    <div class="book-card" onclick="openModal(${idx})" style="animation-delay:${Math.min(idx * 25, 300)}ms">
      <div class="card-cover">
        ${coverImgHtml(c)}
        <div class="card-cover-overlay"></div>
        <div class="card-cover-top">
          <div style="display:flex;align-items:center;gap:6px">
            ${heartBtn}
            <span class="type-badge badge-${c.tip}">${TYPE_LABEL[c.tip]}</span>
          </div>
          <div style="display:flex;align-items:center;gap:5px">
            ${copiesBadge}
            <span class="card-cover-rating">★ ${c.rating.toFixed(1)}</span>
          </div>
        </div>
        <div class="card-cover-bottom">
          <div class="card-title">${esc(c.titlu)}</div>
          <div class="card-author">${esc(c.autori.join(', '))}</div>
          ${extra ? `<div class="card-extra">${esc(extra)}</div>` : ''}
          ${borrowBtn}
        </div>
      </div>
    </div>`;
}

// ── Search & filters ──────────────────────────────────────────────────────────
const searchInput = document.getElementById('search-input');
const searchClear = document.getElementById('search-clear');
let searchTimer;
searchInput.addEventListener('input', () => {
  state.search = searchInput.value.trim();
  searchClear.classList.toggle('hidden', !state.search);
  clearTimeout(searchTimer);
  searchTimer = setTimeout(renderBooks, 180);
});
searchClear.addEventListener('click', () => {
  searchInput.value = ''; state.search = '';
  searchClear.classList.add('hidden'); renderBooks();
});
document.querySelectorAll('.filter-btn').forEach(btn => {
  btn.addEventListener('click', () => {
    document.querySelectorAll('.filter-btn').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    state.filter = btn.dataset.tip;
    renderBooks();
  });
});

// ── Borrow / Return ───────────────────────────────────────────────────────────
async function borrowBook(titlu) {
  try {
    const res = await api('POST', '/api/imprumut', { titlu });
    state.user.imprumuturi = res.imprumuturi;
    _recoKey = '';
    toast('Carte imprumutata! Returneaza in 2 saptamani.', 'success');
    renderBooks();
    renderRecommendations();
  } catch (err) { toast(err.message, 'error'); }
}

async function returnBook(titlu) {
  try {
    const res = await api('DELETE', `/api/imprumut/${encodeURIComponent(titlu)}`);
    state.user.imprumuturi = res.imprumuturi;
    _recoKey = '';
    toast('Carte returnata!', 'success');
    renderMyBooks();
    renderBooks();
    renderRecommendations();
  } catch (err) { toast(err.message, 'error'); }
}

// ── My Books ──────────────────────────────────────────────────────────────────
function renderMyBooks() {
  const imprumuturi = state.user?.imprumuturi || [];
  const list = document.getElementById('mybooks-list');
  const empty = document.getElementById('mybooks-empty');

  const totalAmenda = imprumuturi.reduce((s, i) => s + calcAmenda(i), 0);
  const subtitle = document.getElementById('mybooks-subtitle');
  subtitle.innerHTML = `${imprumuturi.length} carte${imprumuturi.length !== 1 ? 'i' : ''} imprumutata${imprumuturi.length !== 1 ? 'te' : ''}` +
    (totalAmenda > 0 ? ` &nbsp;·&nbsp; <span style="color:var(--danger);font-weight:700">💰 Amenda totala: ${totalAmenda.toFixed(2)} RON</span>` : '');

  if (!imprumuturi.length) { list.innerHTML = ''; empty.classList.remove('hidden'); return; }
  empty.classList.add('hidden');

  const books = state.carti.filter(c => getBorrowedTitles().includes(c.titlu));
  list.innerHTML = books.map((c, i) => {
    const imp = findBorrowed(c.titlu);
    const isFav = getFavorites().includes(c.titlu);
    const heartBtn = `<button class="card-heart${isFav ? ' active' : ''}" onclick="event.stopPropagation(); toggleFavorite('${esc(c.titlu)}')">${isFav ? '❤️' : '🤍'}</button>`;

    return `
      <div class="book-card" style="animation-delay:${i * 30}ms">
        <div class="card-cover">
          ${coverImgHtml(c)}
          <div class="card-cover-overlay"></div>
          <div class="card-cover-top">
            <div style="display:flex;align-items:center;gap:6px">
              ${heartBtn}
              <span class="type-badge badge-${c.tip}">${TYPE_LABEL[c.tip]}</span>
            </div>
            <span class="card-cover-rating">★ ${c.rating.toFixed(1)}</span>
          </div>
          <div class="card-cover-bottom">
            <div class="card-title">${esc(c.titlu)}</div>
            <div class="card-author">${esc(c.autori.join(', '))}</div>
            ${imp ? dateStatusHtml(imp) : ''}
            ${calcAmenda(imp) > 0 ? `<div class="amenda-badge">💰 ${calcAmenda(imp).toFixed(2)} RON</div>` : imp?.amendaStinsa ? '<div style="font-size:10px;color:var(--success);margin-top:2px">✓ Amenda stinsa</div>' : ''}
            <button class="borrow-btn" style="margin-top:7px;background:rgba(248,113,113,0.2);color:var(--danger);border:1px solid rgba(248,113,113,0.45);backdrop-filter:blur(6px)"
              onclick="returnBook('${esc(c.titlu)}')">${t('return_btn')}</button>
          </div>
        </div>
      </div>`;
  }).join('');
}

// ── Recommendations ──────────────────────────────────────────────────────────
let _recoCache = null;
let _recoKey = '';

function getRecommendations() {
  if (!state.user || !state.carti.length) return [];
  const borrowed = getBorrowedTitles();
  const favs = getFavorites();
  const interacted = new Set([...borrowed, ...favs]);
  const key = [...interacted].sort().join(',');
  if (key === _recoKey && _recoCache) return _recoCache;
  _recoKey = key;
  if (!interacted.size) { _recoCache = []; return []; }
  const types = new Set(state.carti.filter(c => interacted.has(c.titlu)).map(c => c.tip));
  const pool = state.carti.filter(c => types.has(c.tip) && !interacted.has(c.titlu));
  _recoCache = [...pool].sort(() => Math.random() - 0.5).slice(0, 14);
  return _recoCache;
}

function renderRecommendations() {
  const section = document.getElementById('reco-section');
  if (!section) return;
  const reco = getRecommendations();
  if (!reco.length) { section.className = 'hidden'; return; }
  window._recoBooks = reco;
  const types = [...new Set(state.carti
    .filter(c => [...getBorrowedTitles(), ...getFavorites()].includes(c.titlu))
    .map(c => TYPE_LABEL[c.tip]))].join(', ');
  section.className = 'reco-section';
  section.innerHTML = `
    <div class="reco-header">
      <span class="reco-heading">🎯 Recomandate pentru tine</span>
      <span class="reco-sub">bazat pe ${types}</span>
    </div>
    <div class="reco-scroll">
      ${reco.map((c, i) => `
        <div class="reco-card" onclick="openRecoModal(${i})">
          <div class="reco-cover">
            ${coverImgHtml(c)}
            <div class="card-cover-overlay"></div>
            <span class="reco-rating">★ ${c.rating.toFixed(1)}</span>
            <div class="reco-info">
              <div class="reco-title">${esc(c.titlu)}</div>
              <div class="reco-author">${esc(c.autori[0] || '')}</div>
            </div>
          </div>
        </div>`).join('')}
    </div>`;
}

function openRecoModal(idx) {
  openModalDirect(window._recoBooks?.[idx]);
}

// ── Book detail modal ─────────────────────────────────────────────────────────
function openModal(idx) {
  openModalDirect(filteredBooks()[idx]);
}

function openModalDirect(c) {
  if (!c) return;

  const imp = findBorrowed(c.titlu);
  const isBorrowed = !!imp;

  let extraDetails = '';
  if (c.tip === 'CarteDigitala') extraDetails = `
    <div class="detail-item"><div class="detail-label">Format</div><div class="detail-value">${c.formatFisier}</div></div>
    <div class="detail-item"><div class="detail-label">DRM</div><div class="detail-value">${c.areDrm ? 'Da' : 'Nu'}</div></div>`;
  else if (c.tip === 'Audiobook') extraDetails = `
    <div class="detail-item"><div class="detail-label">Narator</div><div class="detail-value">${esc(c.narator)}</div></div>
    <div class="detail-item"><div class="detail-label">Durata</div><div class="detail-value">${Math.floor(c.durataMinute/60)}h ${c.durataMinute%60}m</div></div>`;
  else if (c.tip === 'MangaComics') extraDetails = `
    <div class="detail-item"><div class="detail-label">Stil</div><div class="detail-value">${esc(c.stilDesen)}</div></div>
    <div class="detail-item"><div class="detail-label">Color</div><div class="detail-value">${c.esteColor ? 'Da' : 'Nu'}</div></div>`;
  else if (c.tip === 'FilmCarte') extraDetails = `
    <div class="detail-item"><div class="detail-label">Regizor</div><div class="detail-value">${esc(c.regizor)}</div></div>
    <div class="detail-item"><div class="detail-label">An</div><div class="detail-value">${c.anLansare}</div></div>`;

  let borrowSection = '';
  if (isBorrowed && imp) {
    const days = daysUntil(imp.dataReturnare);
    borrowSection = `
      <div style="background:var(--surface-2);border-radius:var(--radius-sm);padding:14px;margin-bottom:20px">
        <div style="font-size:12px;color:var(--text-muted);margin-bottom:6px">IMPRUMUTAT DE TINE</div>
        <div style="font-size:13px;display:flex;flex-direction:column;gap:4px">
          <span>📅 Luat pe: <strong>${fmtDate(imp.dataImprumut)}</strong></span>
          <span>⏰ Returneaza pana la: <strong>${fmtDate(imp.dataReturnare)}</strong></span>
        </div>
        ${dateStatusHtml(imp)}
      </div>`;
  }

  let actionBtn;
  if (!state.user)
    actionBtn = `<button class="btn-ghost" onclick="closeModal(); document.getElementById('login-shortcut').click()">${t('login_to_borrow')}</button>`;
  else if (isBorrowed)
    actionBtn = `<button class="btn-danger" style="padding:10px 20px" onclick="returnBook('${esc(c.titlu)}').then(() => closeModal())">${t('return_btn')}</button>`;
  else
    actionBtn = `<button class="btn-primary" onclick="borrowBook('${esc(c.titlu)}').then(() => closeModal())">${t('borrow_btn')}</button>`;

  const modalCoverHtml = coverUrl(c)
    ? `<img src="${esc(coverUrl(c))}" class="modal-cover" alt="${esc(c.titlu)}" onerror="this.className='modal-cover-placeholder ph-${c.tip}'; this.outerHTML='<div class=\\'modal-cover-placeholder ph-${c.tip}\\'>${TYPE_EMOJI[c.tip]}</div>'" />`
    : `<div class="modal-cover-placeholder ph-${c.tip}">${TYPE_EMOJI[c.tip]}</div>`;

  // ── Reviews section ──
  const recenzii = c.recenzii || [];
  const myReview = state.user ? recenzii.find(r => r.username === state.user.username) : null;
  window._modalTitlu = c.titlu;
  window._selectedStarRating = myReview?.rating || 0;

  const avgRating = recenzii.length
    ? (recenzii.reduce((s, r) => s + r.rating, 0) / recenzii.length).toFixed(1) : null;
  const starsDisplay = n => '★'.repeat(n) + '☆'.repeat(5 - n);

  const reviewsListHtml = [...recenzii]
    .sort((a, b) => b.data.localeCompare(a.data))
    .map(r => `
      <div class="review-item${r.username === state.user?.username ? ' my-review' : ''}">
        <div class="review-meta">
          <span class="review-username">@${esc(r.username)}</span>
          <span class="review-stars">${starsDisplay(r.rating)}</span>
          <span class="review-date">${fmtDate(r.data)}</span>
        </div>
        ${r.comment ? `<div class="review-comment">${esc(r.comment)}</div>` : ''}
      </div>`).join('') || '<div class="review-empty">Nicio recenzie inca. Fii primul!</div>';

  const starBtns = [1,2,3,4,5].map(n =>
    `<button type="button" class="star-btn${window._selectedStarRating >= n ? ' on' : ''}" onclick="setStarRating(${n})">★</button>`
  ).join('');

  const reviewFormHtml = state.user ? `
    <div class="review-form">
      <div class="review-form-title">${myReview ? 'Recenzia ta' : 'Lasa o recenzie'}</div>
      <div class="star-input">${starBtns}</div>
      <textarea id="review-text" rows="2" placeholder="Comentariul tau (optional)...">${esc(myReview?.comment || '')}</textarea>
      <div style="display:flex;gap:8px;flex-wrap:wrap">
        <button class="btn-primary small" onclick="submitReview()">Salveaza</button>
        ${myReview ? `<button class="btn-danger" onclick="deleteReview()">Sterge recenzia</button>` : ''}
      </div>
    </div>` : `<div class="review-login">Autentifica-te pentru a lasa o recenzie.</div>`;

  const reviewsSection = `
    <div class="reviews-section">
      <div class="reviews-header">
        <span class="reviews-title">Recenzii</span>
        ${avgRating ? `<span class="reviews-avg">★ ${avgRating} · ${recenzii.length} recenzi${recenzii.length === 1 ? 'e' : 'i'}</span>` : ''}
      </div>
      <div class="reviews-list">${reviewsListHtml}</div>
      ${reviewFormHtml}
    </div>`;

  document.getElementById('modal-content').innerHTML = `
    ${modalCoverHtml}
    <div class="modal-type"><span class="type-badge badge-${c.tip}">${TYPE_LABEL[c.tip]}</span></div>
    <div class="modal-title">${esc(c.titlu)}</div>
    <div class="modal-authors">${esc(c.autori.join(', '))}</div>
    <div class="modal-stats">
      <div class="stat"><div class="stat-label">Rating</div><div class="stat-value rating">★ ${c.rating.toFixed(1)}</div></div>
      <div class="stat"><div class="stat-label">Volum</div><div class="stat-value">${c.volum}</div></div>
      <div class="stat"><div class="stat-label">Editie</div><div class="stat-value">${c.editie}</div></div>
      ${c.copii != null ? `<div class="stat"><div class="stat-label">Copii</div><div class="stat-value" style="color:${(c.copiiDisponibile??c.copii)<=0?'var(--danger)':(c.copiiDisponibile??c.copii)<=5?'var(--warning)':'var(--success)'}">${c.copiiDisponibile ?? c.copii} / ${c.copii}</div></div>` : ''}
      ${c.serie ? `<div class="stat"><div class="stat-label">Serie</div><div class="stat-value">${esc(c.serie)}</div></div>` : ''}
    </div>
    <p class="modal-desc">${esc(c.detalii)}</p>
    <div class="modal-details">
      <div class="detail-item"><div class="detail-label">ISBN</div><div class="detail-value" style="font-size:12px">${esc(c.isbn)}</div></div>
      ${extraDetails}
    </div>
    ${borrowSection}
    <div class="modal-actions">${actionBtn}</div>
    ${reviewsSection}`;

  document.getElementById('modal-overlay').classList.remove('hidden');
  document.body.style.overflow = 'hidden';
}

function setStarRating(n) {
  window._selectedStarRating = n;
  document.querySelectorAll('.star-btn').forEach((btn, i) => btn.classList.toggle('on', i < n));
}

async function submitReview() {
  const rating = window._selectedStarRating || 0;
  if (!rating) { toast('Alege un rating (1-5 stele)', 'error'); return; }
  const comment = document.getElementById('review-text')?.value.trim() || '';
  const titlu = window._modalTitlu;
  try {
    await api('POST', `/api/recenzii/${encodeURIComponent(titlu)}`, { rating, comment });
    toast('Recenzie salvata!', 'success');
    await loadCarti();
    const idx = filteredBooks().findIndex(b => b.titlu === titlu);
    if (idx !== -1) openModal(idx);
  } catch (err) { toast(err.message, 'error'); }
}

async function deleteReview() {
  const titlu = window._modalTitlu;
  try {
    await api('DELETE', `/api/recenzii/${encodeURIComponent(titlu)}`);
    toast('Recenzie stearsa!', 'success');
    await loadCarti();
    const idx = filteredBooks().findIndex(b => b.titlu === titlu);
    if (idx !== -1) openModal(idx);
  } catch (err) { toast(err.message, 'error'); }
}

function closeModal(e) {
  if (e && e.target !== document.getElementById('modal-overlay')) return;
  document.getElementById('modal-overlay').classList.add('hidden');
  document.body.style.overflow = '';
}

document.addEventListener('keydown', e => {
  if (e.key === 'Escape') { closeModal(); closeEditOverlay(); }
});

// ── Edit overlay ──────────────────────────────────────────────────────────────
function openEditOverlay(html) {
  document.getElementById('edit-content').innerHTML = html;
  document.getElementById('edit-overlay').classList.remove('hidden');
  document.body.style.overflow = 'hidden';
}

function closeEditOverlay(e) {
  if (e && e.target !== document.getElementById('edit-overlay')) return;
  document.getElementById('edit-overlay').classList.add('hidden');
  document.body.style.overflow = '';
}

// ── Edit book ─────────────────────────────────────────────────────────────────
function editBook(titluOriginal) {
  const c = state.carti.find(b => b.titlu === titluOriginal);
  if (!c) return;

  const extraFieldsHtml = {
    CarteDigitala: `
      <div class="field-row">
        <div class="field"><label>Format</label><select id="e-format">
          ${['EPUB','PDF','MOBI','AZW'].map(f => `<option${c.formatFisier===f?' selected':''}>${f}</option>`).join('')}
        </select></div>
        <div class="field"><label>DRM</label><select id="e-drm">
          <option value="false"${!c.areDrm?' selected':''}>Nu</option>
          <option value="true"${c.areDrm?' selected':''}>Da</option>
        </select></div>
      </div>`,
    Audiobook: `
      <div class="field-row">
        <div class="field"><label>Narator</label><input type="text" id="e-narator" value="${esc(c.narator||'')}" /></div>
        <div class="field"><label>Durata (min)</label><input type="number" id="e-durata" value="${c.durataMinute||0}" /></div>
      </div>`,
    MangaComics: `
      <div class="field-row">
        <div class="field"><label>Stil</label><select id="e-stil">
          ${['Manga','Marvel','DC'].map(s => `<option${c.stilDesen===s?' selected':''}>${s}</option>`).join('')}
        </select></div>
        <div class="field"><label>Color</label><select id="e-color">
          <option value="false"${!c.esteColor?' selected':''}>Nu</option>
          <option value="true"${c.esteColor?' selected':''}>Da</option>
        </select></div>
      </div>`,
    FilmCarte: `
      <div class="field-row">
        <div class="field"><label>Regizor</label><input type="text" id="e-regizor" value="${esc(c.regizor||'')}" /></div>
        <div class="field"><label>An lansare</label><input type="number" id="e-an" value="${c.anLansare||''}" /></div>
      </div>`,
  };

  openEditOverlay(`
    <div class="edit-title">✏️ Editeaza carte</div>
    <form class="edit-form" onsubmit="saveBookEdit(event, '${esc(titluOriginal)}')">
      <div class="field"><label>Tip</label>
        <select id="e-tip">
          ${['CarteDigitala','Audiobook','MangaComics','FilmCarte'].map(t =>
            `<option value="${t}"${c.tip===t?' selected':''}>${t}</option>`).join('')}
        </select>
      </div>
      <div class="field"><label>Titlu</label><input type="text" id="e-titlu" value="${esc(c.titlu)}" required /></div>
      <div class="field"><label>Autori <small>(virgula)</small></label><input type="text" id="e-autori" value="${esc(c.autori.join(', '))}" /></div>
      <div class="field-row">
        <div class="field"><label>ISBN</label><input type="text" id="e-isbn" value="${esc(c.isbn)}" /></div>
        <div class="field"><label>Rating</label><input type="number" id="e-rating" value="${c.rating}" min="0" max="10" step="0.1" /></div>
      </div>
      <div class="field-row">
        <div class="field"><label>Volum</label><input type="number" id="e-volum" value="${c.volum}" min="1" /></div>
        <div class="field"><label>Parte</label><input type="number" id="e-parte" value="${c.parte}" min="1" /></div>
        <div class="field"><label>Editie</label><input type="number" id="e-editie" value="${c.editie}" min="1" /></div>
        <div class="field"><label>Copii <small>(nr. ex.)</small></label><input type="number" id="e-copii" value="${c.copii ?? ''}" min="1" placeholder="∞" /></div>
      </div>
      <div class="field"><label>Serie</label><input type="text" id="e-serie" value="${esc(c.serie||'')}" /></div>
      <div class="field"><label>Detalii</label><textarea id="e-detalii" rows="3">${esc(c.detalii)}</textarea></div>
      <div class="field">
        <label>Coperta <small>(URL — lasati gol pentru auto din ISBN)</small></label>
        <input type="url" id="e-imagine" value="${esc(c.imagine||'')}" placeholder="https://..." />
        ${coverUrl(c) ? `<img src="${esc(coverUrl(c))}" style="margin-top:8px;height:80px;border-radius:6px;object-fit:cover" onerror="this.style.display='none'" />` : ''}
      </div>
      ${extraFieldsHtml[c.tip] || ''}
      <div style="display:flex;gap:10px;margin-top:4px">
        <button type="submit" class="btn-primary" style="flex:1">Salveaza</button>
        <button type="button" class="btn-ghost" onclick="closeEditOverlay()">Anuleaza</button>
      </div>
    </form>`);
}

async function saveBookEdit(e, titluOriginal) {
  e.preventDefault();
  const tip = document.getElementById('e-tip').value;
  const autori = document.getElementById('e-autori').value.split(',').map(a => a.trim()).filter(Boolean);

  const copiiEdit = parseInt(document.getElementById('e-copii').value);
  const carte = {
    tip,
    titlu: document.getElementById('e-titlu').value.trim(),
    autori,
    isbn: document.getElementById('e-isbn').value.trim(),
    volum: parseInt(document.getElementById('e-volum').value) || 1,
    parte: parseInt(document.getElementById('e-parte').value) || 1,
    editie: parseInt(document.getElementById('e-editie').value) || 1,
    detalii: document.getElementById('e-detalii').value.trim(),
    rating: parseFloat(document.getElementById('e-rating').value) || 0,
    serie: document.getElementById('e-serie').value.trim(),
    imagine: document.getElementById('e-imagine').value.trim(),
    ...(copiiEdit > 0 && { copii: copiiEdit }),
  };

  if (tip === 'CarteDigitala') {
    carte.formatFisier = document.getElementById('e-format').value;
    carte.areDrm = document.getElementById('e-drm').value === 'true';
  } else if (tip === 'Audiobook') {
    carte.narator = document.getElementById('e-narator').value.trim();
    carte.durataMinute = parseInt(document.getElementById('e-durata').value) || 0;
  } else if (tip === 'MangaComics') {
    carte.stilDesen = document.getElementById('e-stil').value;
    carte.esteColor = document.getElementById('e-color').value === 'true';
  } else if (tip === 'FilmCarte') {
    carte.regizor = document.getElementById('e-regizor').value.trim();
    carte.anLansare = parseInt(document.getElementById('e-an').value) || 0;
  }

  try {
    await api('PUT', `/api/carti/${encodeURIComponent(titluOriginal)}`, carte);
    toast('Carte actualizata!', 'success');
    closeEditOverlay();
    await loadCarti();
    renderAdminBooks();
  } catch (err) { toast(err.message, 'error'); }
}

// ── Edit user ─────────────────────────────────────────────────────────────────
function editUser(username) {
  const conturi = window._adminConturi || [];
  const u = conturi.find(c => c.username === username);
  if (!u) return;

  const adminExtra = u.esteAdmin ? `
    <div class="field-row">
      <div class="field"><label>Email</label><input type="email" id="eu-email" value="${esc(u.email||'')}" placeholder="email@biblioteca.ro" /></div>
      <div class="field"><label>Telefon</label><input type="text" id="eu-telefon" value="${esc(u.telefon||'')}" placeholder="07xx xxx xxx" /></div>
    </div>
    <div class="field-row">
      <div class="field"><label>Functie</label>
        <select id="eu-functie">
          ${['Director','Bibliotecar','Bibliotecar Sef','Administrator Sistem','Manager','Alt rol'].map(f =>
            `<option value="${f}"${(u.functie||'')=== f?' selected':''}>${f}</option>`).join('')}
        </select>
      </div>
      <div class="field"><label>Salariu (RON)</label><input type="number" id="eu-salariu" value="${u.salariu ?? ''}" placeholder="3000" min="0" step="100" /></div>
    </div>` : '';

  openEditOverlay(`
    <div class="edit-title">✏️ Editeaza ${u.esteAdmin ? 'admin' : 'utilizator'}</div>
    <div style="color:var(--text-muted);font-size:13px;margin-bottom:20px">@${esc(username)}</div>
    <form class="edit-form" onsubmit="saveUserEdit(event, '${esc(username)}')">
      <div class="field-row">
        <div class="field"><label>Prenume</label><input type="text" id="eu-prenume" value="${esc(u.prenume)}" /></div>
        <div class="field"><label>Nume</label><input type="text" id="eu-nume" value="${esc(u.nume)}" /></div>
      </div>
      <div class="field"><label>Data nasterii</label><input type="text" id="eu-data" value="${esc(u.dataNasterii||'')}" placeholder="DD.MM.YYYY" /></div>
      ${adminExtra}
      <div class="field">
        <label>Parola noua <small>(lasati gol pentru a pastra actuala)</small></label>
        <input type="password" id="eu-parola" placeholder="••••••••" autocomplete="new-password" />
      </div>
      <div style="display:flex;gap:10px;margin-top:4px">
        <button type="submit" class="btn-primary" style="flex:1">Salveaza</button>
        <button type="button" class="btn-ghost" onclick="closeEditOverlay()">Anuleaza</button>
      </div>
    </form>`);
}

async function saveUserEdit(e, username) {
  e.preventDefault();
  const salariuEl = document.getElementById('eu-salariu');
  const body = {
    prenume:      document.getElementById('eu-prenume').value.trim(),
    nume:         document.getElementById('eu-nume').value.trim(),
    dataNasterii: document.getElementById('eu-data').value.trim(),
    parola:       document.getElementById('eu-parola').value,
    email:        document.getElementById('eu-email')?.value.trim() || undefined,
    telefon:      document.getElementById('eu-telefon')?.value.trim() || undefined,
    functie:      document.getElementById('eu-functie')?.value || undefined,
    salariu:      salariuEl ? (parseFloat(salariuEl.value) || null) : undefined,
  };
  try {
    await api('PUT', `/api/users/${username}`, body);
    toast('Utilizator actualizat!', 'success');
    closeEditOverlay();
    renderAdminUsers();
  } catch (err) { toast(err.message, 'error'); }
}

// ── Admin ─────────────────────────────────────────────────────────────────────
async function renderAdmin() {
  await loadCarti();
  await renderAdminUsers();
  renderAdminBooks();
  renderAdminStats();
}

function renderAdminStats() {
  const conturi = window._adminConturi || [];
  const total = state.carti.length;
  const totalBorrowed = conturi.reduce((s, u) => s + u.imprumuturi.length, 0);
  const overdue = conturi.reduce((s, u) =>
    s + u.imprumuturi.filter(i => daysUntil(i.dataReturnare) < 0).length, 0);
  const dueSoon = conturi.reduce((s, u) =>
    s + u.imprumuturi.filter(i => { const d = daysUntil(i.dataReturnare); return d >= 0 && d <= 3; }).length, 0);

  const totalAmenzi = conturi.reduce((s, u) => s + u.imprumuturi.reduce((ss, i) => ss + calcAmenda(i), 0), 0);

  document.getElementById('admin-stats').innerHTML = `
    <div class="stat-card"><div class="num">${total}</div><div class="lbl">Carti in catalog</div></div>
    <div class="stat-card"><div class="num">${conturi.length}</div><div class="lbl">Utilizatori</div></div>
    <div class="stat-card"><div class="num">${totalBorrowed}</div><div class="lbl">Carti imprumutate</div></div>
    <div class="stat-card warn"><div class="num">${dueSoon}</div><div class="lbl">Returnare in curand</div></div>
    <div class="stat-card danger"><div class="num">${overdue}</div><div class="lbl">Intarziate</div></div>
    <div class="stat-card danger"><div class="num">${totalAmenzi.toFixed(2)}</div><div class="lbl">RON amenzi neplatite</div></div>`;
}

async function renderAdminUsers() {
  try {
    const { conturi } = await api('GET', '/api/users');
    window._adminConturi = conturi;
    renderAdminStats();

    document.getElementById('users-list').innerHTML = conturi.map(u => {
      const hasOverdue = u.imprumuturi.some(i => daysUntil(i.dataReturnare) < 0);
      const userTotalAmenda = u.imprumuturi.reduce((s, i) => s + calcAmenda(i), 0);
      const impHtml = u.imprumuturi.length ? `
        <div class="user-imprumuturi">
          ${u.imprumuturi.map(i => {
            const days = daysUntil(i.dataReturnare);
            const cls = days < 0 ? 'date-overdue' : days <= 3 ? 'date-warn' : 'date-ok';
            const amenda = calcAmenda(i);
            return `<div class="imp-entry">
              <span class="imp-title">${esc(i.titlu)}</span>
              <span class="imp-dates">${fmtDate(i.dataImprumut)} → ${fmtDate(i.dataReturnare)}</span>
              <span class="date-status ${cls}" style="font-size:11px;padding:2px 7px">${days < 0 ? `⚠ +${Math.abs(days)}z` : `${days}z`}</span>
              ${amenda > 0 ? `<span class="amenda-badge" style="font-size:10px;padding:2px 7px">💰 ${amenda.toFixed(2)} RON</span>` : i.amendaStinsa ? '<span style="font-size:10px;color:var(--success)">✓ stinsa</span>' : ''}
              ${amenda > 0 ? `<button class="btn-stinge" onclick="stingeAmenda('${esc(u.username)}','${esc(i.titlu)}')">Stinge</button>` : ''}
            </div>`;
          }).join('')}
          ${userTotalAmenda > 0 ? `<div style="font-size:12px;color:var(--danger);font-weight:700;padding-top:6px;border-top:1px solid var(--border)">Total amenda: ${userTotalAmenda.toFixed(2)} RON</div>` : ''}
        </div>` : '';

      const isDir = u.functie === 'Director';
      const badgeCls = isDir ? 'director' : u.esteAdmin ? 'admin' : 'user';
      const badgeTxt = isDir ? 'DIRECTOR' : u.esteAdmin ? 'ADMIN' : 'user';
      return `<div class="user-row">
        <div class="user-row-top">
          <div class="user-info-text">
            <div class="user-name">
              ${esc(u.prenume)} ${esc(u.nume)}
              <span class="user-badge ${badgeCls}">${badgeTxt}</span>
              ${hasOverdue ? '<span style="color:var(--danger);font-size:12px">⚠ intarziat</span>' : ''}
            </div>
            <div class="user-meta">@${esc(u.username)} · ${u.dataNasterii || '—'}${u.functie ? ' · ' + esc(u.functie) : ''}${u.email ? ' · ' + esc(u.email) : ''}${u.telefon ? ' · ' + esc(u.telefon) : ''}${u.salariu ? ' · 💰 ' + u.salariu + ' RON' : ''} · ${u.imprumuturi.length} imprumuturi</div>
          </div>
          <div class="user-row-actions">
            ${isDirector() ? `<button class="btn-edit" onclick="editUser('${esc(u.username)}')">✏ Edit</button>` : ''}
            ${isDirector() && !u.esteAdmin ? `<button class="btn-danger" onclick="deleteUser('${esc(u.username)}')">✕</button>` : ''}
          </div>
        </div>
        ${impHtml}
      </div>`;
    }).join('');
  } catch (err) { toast('Eroare la incarcarea userilor', 'error'); }
}

async function renderAdminBooks(filter = '') {
  const list = document.getElementById('admin-books-list');
  let books = state.carti;
  if (filter) {
    const q = filter.toLowerCase();
    books = books.filter(c =>
      c.titlu.toLowerCase().includes(q) || c.autori.some(a => a.toLowerCase().includes(q))
    );
  }
  list.innerHTML = books.map(c => `
    <div class="admin-book-row">
      <span class="type-badge badge-${c.tip}" style="font-size:11px">${TYPE_LABEL[c.tip]}</span>
      <div class="admin-book-info">
        <div class="admin-book-title">${esc(c.titlu)}</div>
        <div class="admin-book-author">${esc(c.autori.join(', '))} · ★ ${c.rating.toFixed(1)}${c.copii != null ? ` · 📚 ${c.copiiDisponibile ?? c.copii}/${c.copii} copii` : ''}</div>
      </div>
      <div class="row-actions">
        <button class="btn-edit" onclick="editBook('${esc(c.titlu)}')">✏ Edit</button>
        <button class="btn-danger" onclick="deleteBook('${esc(c.titlu)}')">✕</button>
      </div>
    </div>`).join('');
}

document.getElementById('admin-search').addEventListener('input', function () {
  renderAdminBooks(this.value.trim());
});

async function fetchCovers() {
  const btn = document.getElementById('fetch-covers-btn');
  btn.disabled = true;
  btn.textContent = '⏳ Se incarca...';
  try {
    const r = await api('POST', '/api/admin/fetch-covers');
    toast(`${r.found} coperte gasite! (${r.skipped} aveau deja)`, 'success');
    await loadCarti();
    renderBooks();
    renderAdminBooks();
  } catch (err) {
    toast(err.message, 'error');
  } finally {
    btn.disabled = false;
    btn.textContent = '🖼 Fetch Covers';
  }
}

async function stingeAmenda(username, titlu) {
  try {
    await api('POST', '/api/amenzi/stinge', { username, titlu });
    toast('Amenda stinsa!', 'success');
    renderAdminUsers();
  } catch (err) { toast(err.message, 'error'); }
}

async function deleteBook(titlu) {
  if (!confirm(`Stergi "${titlu}"?`)) return;
  try {
    await api('DELETE', `/api/carti/${encodeURIComponent(titlu)}`);
    toast('Carte stearsa', 'success');
    await loadCarti();
    renderAdminBooks();
    renderAdminStats();
  } catch (err) { toast(err.message, 'error'); }
}

async function deleteUser(username) {
  if (!confirm(`Stergi utilizatorul @${username}?`)) return;
  try {
    await api('DELETE', `/api/users/${username}`);
    toast('Utilizator sters', 'success');
    renderAdminUsers();
  } catch (err) { toast(err.message, 'error'); }
}

// ── Add-book mode tabs ────────────────────────────────────────────────────────
document.querySelectorAll('.add-mode-tab').forEach(btn => {
  btn.addEventListener('click', () => {
    document.querySelectorAll('.add-mode-tab').forEach(b => b.classList.remove('active'));
    btn.classList.add('active');
    const mode = btn.dataset.mode;
    document.getElementById('add-mode-api').classList.toggle('hidden', mode !== 'api');
    document.getElementById('add-mode-manual').classList.toggle('hidden', mode !== 'manual');
  });
});

function switchToManual() {
  document.querySelectorAll('.add-mode-tab').forEach(b => b.classList.remove('active'));
  document.querySelector('.add-mode-tab[data-mode="manual"]').classList.add('active');
  document.getElementById('add-mode-api').classList.add('hidden');
  document.getElementById('add-mode-manual').classList.remove('hidden');
}

// ── API search ────────────────────────────────────────────────────────────────
window._apiResults = [];

document.getElementById('api-query').addEventListener('keydown', e => {
  if (e.key === 'Enter') { e.preventDefault(); doApiSearch(); }
});

async function doApiSearch() {
  const q = document.getElementById('api-query').value.trim();
  const type = document.getElementById('api-type').value;
  const box = document.getElementById('api-results');
  if (!q) return;

  box.innerHTML = `<div class="api-spinner">⏳ Se cauta...</div>`;
  window._apiResults = [];

  try {
    const data = await api('GET', `/api/search/${type}?q=${encodeURIComponent(q)}`);
    window._apiResults = data.results || [];
    renderApiResults(type);
  } catch (err) {
    if (err.message === 'OMDB_KEY_MISSING') {
      box.innerHTML = `<div class="api-key-warning">
        ⚠️ <strong>Cheia OMDB nu e configurata.</strong><br>
        1. Obtine o cheie gratuita la <strong>omdbapi.com/apikey.aspx</strong><br>
        2. Deschide <code>web/server.js</code><br>
        3. Inlocuieste <code>YOUR_OMDB_KEY</code> cu cheia ta<br>
        4. Reporneste serverul
      </div>`;
    } else {
      box.innerHTML = `<div class="api-empty">❌ ${esc(err.message)}</div>`;
    }
  }
}

function renderApiResults(type) {
  const box = document.getElementById('api-results');
  const results = window._apiResults;
  if (!results.length) {
    box.innerHTML = `<div class="api-empty">Niciun rezultat gasit.</div>`;
    return;
  }

  box.innerHTML = results.map((r, i) => {
    const coverHtml = r.imagine
      ? `<img src="${esc(r.imagine)}" class="api-result-cover" onerror="this.className='api-result-cover-ph ph-${type==='film'?'FilmCarte':'CarteDigitala'}'; this.outerHTML='<div class=\\'api-result-cover-ph ph-${type==='film'?'FilmCarte':'CarteDigitala'}\\'>${type==='film'?'🎬':'📚'}</div>'" />`
      : `<div class="api-result-cover-ph ph-${type==='film'?'FilmCarte':'CarteDigitala'}">${type==='film'?'🎬':'📚'}</div>`;

    const meta = type === 'film'
      ? `${r.regizor ? esc(r.regizor) + ' · ' : ''}${r.anLansare || ''}${r.rating ? ' · ★ ' + r.rating.toFixed(1) : ''}`
      : `${r.autori.slice(0,2).map(esc).join(', ')}${r.an ? ' · ' + r.an : ''}`;

    return `
      <div class="api-result-card">
        ${coverHtml}
        <div class="api-result-info">
          <div class="api-result-title">${esc(r.titlu)}</div>
          <div class="api-result-meta">${meta}</div>
          ${r.detalii ? `<div class="api-result-desc">${esc(r.detalii)}</div>` : ''}
          <div class="api-result-actions">
            <button class="btn-primary small" onclick="importApiResult(${i}, '${type}')">+ Importa</button>
          </div>
        </div>
      </div>`;
  }).join('');
}

function importApiResult(idx, type) {
  const r = window._apiResults[idx];
  if (!r) return;

  switchToManual();

  // set type
  const tipEl = document.getElementById('add-tip');
  if (type === 'film') {
    tipEl.value = 'FilmCarte';
  }
  // keep whatever tip user had selected for books (they may want Audiobook/MangaComics)
  document.querySelectorAll('.extra-fields').forEach(el => el.classList.add('hidden'));
  document.getElementById(`extra-${tipEl.value}`).classList.remove('hidden');

  // fill common fields
  document.getElementById('add-titlu').value  = r.titlu || '';
  document.getElementById('add-autori').value  = (r.autori || []).join(', ');
  document.getElementById('add-isbn').value    = r.isbn || '';
  document.getElementById('add-serie').value   = r.serie || '';
  document.getElementById('add-detalii').value = r.detalii || '';
  document.getElementById('add-imagine').value = r.imagine || '';
  document.getElementById('add-rating').value  = r.rating > 0 ? r.rating : '';
  document.getElementById('add-editie').value  = r.editie || 1;

  // fill film-specific fields
  if (type === 'film') {
    document.getElementById('add-regizor').value = r.regizor || '';
    document.getElementById('add-an').value       = r.anLansare || '';
  }

  toast('Date importate! Verifica si completeaza campurile lipsa.', 'success');

  // scroll form into view
  document.getElementById('add-mode-manual').scrollIntoView({ behavior: 'smooth', block: 'start' });
}

// Add book form — toggle extra fields
document.getElementById('add-tip').addEventListener('change', function () {
  document.querySelectorAll('.extra-fields').forEach(el => el.classList.add('hidden'));
  document.getElementById(`extra-${this.value}`).classList.remove('hidden');
});

document.getElementById('add-book-form').addEventListener('submit', async e => {
  e.preventDefault();
  const tip = document.getElementById('add-tip').value;
  const autori = document.getElementById('add-autori').value.split(',').map(a => a.trim()).filter(Boolean);
  const copiiVal = parseInt(document.getElementById('add-copii').value);
  const carte = {
    tip, autori,
    titlu: document.getElementById('add-titlu').value.trim(),
    isbn: document.getElementById('add-isbn').value.trim(),
    volum: parseInt(document.getElementById('add-volum').value) || 1,
    parte: parseInt(document.getElementById('add-parte').value) || 1,
    editie: parseInt(document.getElementById('add-editie').value) || 1,
    detalii: document.getElementById('add-detalii').value.trim(),
    rating: parseFloat(document.getElementById('add-rating').value) || 0,
    serie: document.getElementById('add-serie').value.trim(),
    imagine: document.getElementById('add-imagine').value.trim(),
    ...(copiiVal > 0 && { copii: copiiVal }),
  };
  if (tip === 'CarteDigitala') { carte.formatFisier = document.getElementById('add-format').value; carte.areDrm = document.getElementById('add-drm').value === 'true'; }
  else if (tip === 'Audiobook') { carte.narator = document.getElementById('add-narator').value.trim(); carte.durataMinute = parseInt(document.getElementById('add-durata').value) || 0; }
  else if (tip === 'MangaComics') { carte.stilDesen = document.getElementById('add-stil').value; carte.esteColor = document.getElementById('add-color').value === 'true'; }
  else if (tip === 'FilmCarte') { carte.regizor = document.getElementById('add-regizor').value.trim(); carte.anLansare = parseInt(document.getElementById('add-an').value) || 0; }

  try {
    await api('POST', '/api/carti', carte);
    toast('Carte adaugata!', 'success');
    e.target.reset();
    await loadCarti();
    renderAdminBooks();
  } catch (err) { toast(err.message, 'error'); }
});

// ── Utils ─────────────────────────────────────────────────────────────────────
function esc(str) {
  return String(str ?? '')
    .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;').replace(/'/g, '&#39;');
}

// ── Boot ──────────────────────────────────────────────────────────────────────
tryRestoreSession();
