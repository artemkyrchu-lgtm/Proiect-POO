const fs = require('fs');
const path = require('path');

const BIBLIOTECA_PATH = path.join(__dirname, '..', 'biblioteca.json');

const usernames = [
  'bibliofil', 'cititoarea_sofia', 'marcel_b', 'ioana_citeste', 'alex_ro',
  'stefan_m', 'diana_carti', 'radu_v', 'ana_pop', 'mihai_c',
  'tudor_r', 'elena_d', 'gabi_reads', 'cristina_c', 'vlad_b',
  'andrei_t', 'maria_l', 'dan_s', 'roxana_p', 'cosmin_f',
];

const comments = {
  general: [
    'Fantastic! M-a captivat de la prima pagina.',
    'O lectura excelenta, recomand cu caldura!',
    'Una dintre cele mai bune din colectie.',
    'M-a tinut treaz toata noaptea, nu puteam sa o las din mana.',
    'Merita fiecare pagina. O capodopera!',
    'Exact ce cautam. Foarte bine scrisa.',
    'Impresionanta! Mi-a schimbat perspectiva.',
    'Nu ma asteptam sa imi placa atat de mult, dar a fost superba.',
    'O lectura usoara si placuta, potrivita pentru orice moment.',
    'Destul de buna, dar are si momente mai lente pe ici pe colo.',
    'OK, se poate citi, dar nu mi s-a parut ceva special.',
    'Ma asteptam la mai mult sincer, putin dezamagit.',
    'Nu e pentru gustul meu, dar inteleg de ce altora le place.',
    'Interesanta pe alocuri, dar cam lunga pentru povestea pe care o spune.',
    'Recomand tuturor prietenilor mei!',
    'Am citit-o intr-o singura zi, nu m-am putut opri.',
    'Scriitura extraordinara, personaje memorabile si bine construite.',
    'O poveste care ramane cu tine mult dupa ce termini ultima pagina.',
    'Surprinzator de buna fata de asteptari!',
    'Exact genul de poveste de care aveam nevoie.',
    'Buna, dar nu extraordinara. O recitesc candva.',
    'Un titlu care merita sa fie mai cunoscut.',
    'Povestea are ritm bun si te tine ancorat.',
    'Finalul m-a lasat cu gura cascata.',
    'Am recomandat-o deja la trei prieteni.',
  ],
  MangaComics: [
    'Ilustratiile sunt absolut superbe, fiecare pagina e o opera de arta!',
    'Artwork-ul e impresionant, povestea si mai si.',
    'Seria trebuie citita neaparat in ordine, altfel pierzi mult.',
    'Cel mai bun manga pe care l-am citit pana acum.',
    'Desenele si povestea se completeaza perfect.',
    'Am citit tot volumul intr-o ora, e atat de captivant.',
    'Stilul grafic e unic si imediat recognoscibil.',
    'Emotia transmisa prin desene e remarcabila.',
  ],
  FilmCarte: [
    'Regie impecabila, distributie perfecta.',
    'Un film de neratat, pur si simplu!',
    'Cinematografie extraordinara, fiecare cadru e gandit.',
    'Povestea e tulburatoare si realista.',
    'Unul din filmele care iti ramane in minte mult timp dupa ce il vezi.',
    'Scenariu genial, recomandat 10 din 10.',
    'Coloana sonora completeaza perfect atmosfera.',
    'Performanta actorilor e la cel mai inalt nivel.',
  ],
  Audiobook: [
    'Naratorul are o voce extraordinara, te transports complet in poveste.',
    'Ascultarea e o placere pura, nu te poti opri.',
    'Am ascultat-o pe drum la servici si nu voiam sa ajung.',
    'Naratia adauga un plus enorm fata de versiunea scrisa.',
    'Perfect pentru momentele cand nu poti citi fizic.',
    'Calitatea inregistrarii e excelenta.',
  ],
  CarteDigitala: [
    'Formatul digital e foarte comod, o am mereu cu mine.',
    'Se citeste perfect pe orice dispozitiv.',
    'Versiunea digitala e bine formatata si usor de navigat.',
    'Practica si accesibila, recomand varianta digitala.',
    'Fontul si formatarea sunt placute pentru ochi.',
  ],
};

const ratingWeights = [0, 2, 3, 8, 14, 14]; // index = star count

function randomRating() {
  const total = ratingWeights.reduce((a, b) => a + b, 0);
  let r = Math.floor(Math.random() * total);
  for (let i = 0; i < ratingWeights.length; i++) {
    r -= ratingWeights[i];
    if (r < 0) return i;
  }
  return 5;
}

function randomComment(tip) {
  const pool = [...comments.general, ...(comments[tip] || [])];
  return pool[Math.floor(Math.random() * pool.length)];
}

function randomDate() {
  const now = new Date('2026-06-02');
  const daysBack = Math.floor(Math.random() * 180);
  now.setDate(now.getDate() - daysBack);
  return now.toISOString().split('T')[0];
}

function shuffle(arr) {
  const a = [...arr];
  for (let i = a.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [a[i], a[j]] = [a[j], a[i]];
  }
  return a;
}

const data = JSON.parse(fs.readFileSync(BIBLIOTECA_PATH, 'utf8'));
let added = 0;

for (const carte of data.carti) {
  if (carte.recenzii && carte.recenzii.length > 0) continue;
  const count = 2 + Math.floor(Math.random() * 4); // 2-5 reviews
  const selectedUsers = shuffle(usernames).slice(0, count);
  carte.recenzii = selectedUsers.map(username => ({
    username,
    rating: randomRating(),
    comment: randomComment(carte.tip),
    data: randomDate(),
  }));
  added++;
}

fs.writeFileSync(BIBLIOTECA_PATH, JSON.stringify(data, null, 2));
console.log(`Done! Added reviews to ${added} books.`);
