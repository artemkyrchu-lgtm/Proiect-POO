import zipfile, shutil, os, re

def esc(s):
    return s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;')

SRC = '/mnt/c/Users/cc170/Downloads/ExempluDocumentatie(1).docx'
DST = '/mnt/c/Users/cc170/Downloads/Documentatie_Kyrchu_Artem.docx'

W  = 'http://schemas.openxmlformats.org/wordprocessingml/2006/main'
NS = 'xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"'

def p(text, style=None, bold=False, size=24, center=False, after=200, before=0, italic=False):
    jc = '<w:jc w:val="center"/>' if center else ''
    sp = f'<w:spacing w:before="{before}" w:after="{after}"/>'
    pst = f'<w:pStyle w:val="{style}"/>' if style else ''
    b_open  = '<w:b/><w:bCs/>' if bold else ''
    i_open  = '<w:i/><w:iCs/>' if italic else ''
    sz = f'<w:sz w:val="{size}"/><w:szCs w:val="{size}"/>'
    return (
        f'<w:p>'
        f'<w:pPr>{pst}{sp}{jc}'
        f'<w:rPr><w:rFonts w:ascii="Cambria" w:hAnsi="Cambria" w:cs="Cambria"/>'
        f'{b_open}{i_open}{sz}</w:rPr></w:pPr>'
        f'<w:r><w:rPr><w:rFonts w:ascii="Cambria" w:hAnsi="Cambria" w:cs="Cambria"/>'
        f'{b_open}{i_open}{sz}</w:rPr>'
        f'<w:t xml:space="preserve">{esc(text)}</w:t></w:r>'
        f'</w:p>'
    )

def blank(after=200):
    return f'<w:p><w:pPr><w:spacing w:after="{after}"/></w:pPr></w:p>'

def heading1(text):
    return p(text, bold=True, size=28, center=True, after=240, before=240)

def heading2(text):
    return p(text, bold=True, size=24, after=160, before=160)

def normal(text, after=160):
    return p(text, size=22, after=after)

def bullet(text):
    return (
        '<w:p><w:pPr>'
        '<w:spacing w:after="100"/>'
        '<w:ind w:left="720"/>'
        '</w:pPr>'
        '<w:r><w:rPr>'
        '<w:rFonts w:ascii="Cambria" w:hAnsi="Cambria" w:cs="Cambria"/>'
        '<w:sz w:val="22"/><w:szCs w:val="22"/>'
        '</w:rPr>'
        f'<w:t xml:space="preserve">• {esc(text)}</w:t></w:r></w:p>'
    )

def page_break():
    return '<w:p><w:r><w:br w:type="page"/></w:r></w:p>'

body_parts = []

# ── COPERTĂ ──────────────────────────────────────────────────────────────────
body_parts += [
    p('UNIVERSITATEA "ȘTEFAN CEL MARE", SUCEAVA', bold=True, size=22, center=False, after=0, italic=True),
    p('FACULTATEA DE INGINERIE ELECTRICĂ ȘI ȘTIINȚA CALCULATOARELOR', bold=True, size=22, center=False, after=0, italic=True),
    p('SPECIALIZAREA CALCULATOARE', bold=True, size=22, center=False, after=0, italic=True),
    p('AN 2, GRUPA 3122 A', bold=True, size=22, center=False, after=200, italic=True),
    blank(), blank(), blank(), blank(), blank(),
    p('PROIECT  DISCIPLINA POO', bold=True, size=56, center=True, after=300),
    p('Aplicație Gestiune Bibliotecă', bold=True, size=40, center=True, after=200),
    p('(C++17 + Node.js)', bold=True, size=28, center=True, after=400),
    blank(), blank(), blank(),
    p('Autor: Kyrchu Artem', bold=True, size=24, center=False, after=100),
    p('Profesor coordonator: Remus Prodan', bold=True, size=24, center=False, after=100),
    blank(),
    p('2026', bold=True, size=24, center=True, after=200),
    page_break(),
]

# ── TEMĂ PROIECT ─────────────────────────────────────────────────────────────
body_parts += [
    heading1('TEMĂ PROIECT'),
    heading2('Descriere temă'),
    normal('Aplicație de gestiune a unei biblioteci digitale. Opțiuni disponibile:'),
    blank(100),
    p('Utilizator obișnuit:', bold=True, size=22, after=80),
    bullet('înregistrare / autentificare în sistem'),
    bullet('răsfoire și căutare cărți după titlu, autor, serie sau tip'),
    bullet('împrumutarea și returnarea cărților'),
    bullet('adăugarea cărților la lista de favorite'),
    bullet('vizualizarea cărților împrumutate și a amenzilor'),
    bullet('acordarea de recenzii și rating cărților'),
    blank(100),
    p('Administrator:', bold=True, size=22, after=80),
    bullet('adăugare / editare / ștergere cărți din catalog'),
    bullet('gestionarea conturilor utilizatorilor'),
    bullet('vizualizarea statisticilor bibliotecii'),
    bullet('gestionarea amenzilor utilizatorilor'),
    blank(200),
    heading2('De ce ai ales tema?'),
    normal('Am ales această temă deoarece un sistem de gestiune a bibliotecii reprezintă un exemplu '
           'clasic și complet pentru aplicarea conceptelor de Programare Orientată pe Obiecte: '
           'ierarhie de clase, polimorfism, moștenire și abstractizare. Proiectul îmi permite să '
           'demonstrez aceste concepte într-un context practic și relevant, extinzând aplicația '
           'C++ cu o interfață web modernă construită în Node.js, ceea ce reflectă arhitecturi '
           'reale utilizate în industrie.'),
    page_break(),
]

# ── CUPRINS ───────────────────────────────────────────────────────────────────
body_parts += [
    heading1('CUPRINS'),
    blank(100),
    p('CAPITOLUL I', bold=True, size=22, after=60),
    normal('Descriere proiect din punct de vedere al utilizatorului…………………………..4', after=80),
    blank(60),
    p('CAPITOLUL II', bold=True, size=22, after=60),
    normal('Descriere proiect din punct de vedere al programatorului…………………………7', after=80),
    blank(60),
    p('CAPITOLUL III', bold=True, size=22, after=60),
    normal('Dezvoltări ulterioare……………………………………………………………………10', after=80),
    blank(60),
    p('BIBLIOGRAFIE', bold=True, size=22, after=60),
    page_break(),
]

# ── CAPITOLUL I ───────────────────────────────────────────────────────────────
body_parts += [
    heading1('CAPITOLUL I'),
    heading1('Descriere proiect din punct de vedere al utilizatorului'),
    blank(100),

    heading2('Prezentare generală'),
    normal('Proiectul "Sistem de Gestiune a Bibliotecii" este o aplicație cu două componente: '
           'un modul TUI (Text User Interface) scris în C++17 și o aplicație web completă '
           'construită cu Node.js și JavaScript vanilla. Ambele componente accesează același '
           'set de date (fișiere JSON), oferind utilizatorilor o experiență consistentă '
           'indiferent de interfața aleasă.'),

    heading2('Interfața cu utilizatorul – Aplicația Web'),
    normal('Pagina principală a aplicației web prezintă catalogul complet al bibliotecii, '
           'cu cărți afișate sub formă de carduri. Fiecare card conține: coperta cărții, '
           'titlul, autorul, tipul (Digital, Audiobook, Manga, Film), ratingul și butoanele '
           'de acțiune (Împrumută / Returnează, Adaugă la favorite).'),

    heading2('Funcționalități principale pentru utilizatorul obișnuit'),
    blank(80),
    p('1. Autentificare și înregistrare', bold=True, size=22, after=80),
    normal('Utilizatorul se poate înregistra cu un cont nou completând numele, prenumele, '
           'data nașterii, username-ul și parola. Autentificarea se realizează prin '
           'username și parolă. Există și un mod de navigare ca oaspete (Guest) care '
           'permite răsfoirea catalogului fără cont.'),

    p('2. Catalogul de cărți', bold=True, size=22, after=80),
    normal('Catalogul oferă filtrare pe tipuri (Toate, Digital, Audiobook, Manga, Film, '
           'Favorite), sortare (A-Z, Z-A, după rating, după an) și filtre avansate '
           '(excludere tipuri, rating minim). Funcția de căutare permite găsirea cărților '
           'după titlu, autor sau serie.'),

    p('3. Împrumuturi și returnări', bold=True, size=22, after=80),
    normal('Utilizatorul poate împrumuta orice carte disponibilă. Termenul de returnare '
           'este de 14 zile. Cărțile împrumutate apar în secțiunea "Cărțile mele", '
           'unde sunt afișate data scadenței, avertismente pentru întârzieri și eventualele '
           'amenzi (0,50 RON/zi de întârziere).'),

    p('4. Favorite și recenzii', bold=True, size=22, after=80),
    normal('Cărțile pot fi marcate ca favorite cu un singur click. Utilizatorul poate '
           'lăsa recenzii și rating pentru cărțile pe care le-a citit. Secțiunea '
           '"Recomandate pentru tine" sugerează cărți similare cu cele deja împrumutate '
           'sau marcate ca favorite.'),

    heading2('Funcționalități pentru administrator'),
    normal('Administratorul are acces la un panou special care afișează statistici '
           'generale: numărul total de cărți, utilizatori activi, cărți împrumutate, '
           'cărți scadente în curând, cărți cu întârziere și totalul amenzilor în RON. '
           'Din panoul de administrare se pot adăuga cărți noi (prin căutare automată '
           'în Open Library sau OMDB, sau prin completare manuală), edita sau șterge '
           'cărți existente și gestiona conturile utilizatorilor inclusiv amenzile acestora.'),

    heading2('Interfața TUI (C++)'),
    normal('Aplicația C++ oferă un meniu interactiv în terminal prin care utilizatorul '
           'poate naviga în catalog, împrumuta cărți, gestiona favoritele și, dacă este '
           'administrator, gestiona întreg catalogul. Datele sunt persistate în fișiere '
           'JSON partajate cu aplicația web.'),

    page_break(),
]

# ── CAPITOLUL II ──────────────────────────────────────────────────────────────
body_parts += [
    heading1('CAPITOLUL II'),
    heading1('Descriere proiect din punct de vedere al programatorului'),
    blank(100),

    heading2('Diagrama de clase (C++)'),
    normal('Ierarhia de clase este centrată pe clasa de bază abstractă Carte, din care '
           'derivează patru tipuri specializate de publicații:'),
    blank(80),

    bullet('Carte (clasă abstractă) – câmpuri comune: titlu, autori, isbn, volum, parte, '
           'detalii, rating, editieRevizie, serie. Metode virtuale pure: afisareDetalii(), getTip()'),
    bullet('CarteDigitala : Carte – adaugă: formatFisier (PDF/EPUB), areDrm (bool)'),
    bullet('Audiobook : Carte – adaugă: narator (string), durataMinute (int)'),
    bullet('MangaComics : Carte – adaugă: esteColor (bool), stilDesen (string)'),
    bullet('FilmCarte : Carte – adaugă: regizor (string), anLansare (int)'),
    bullet('Biblioteca – deține un vector<unique_ptr<Carte>> pentru gestionarea catalogului'),
    bullet('Cont (struct) – date utilizator: username, parola, esteAdmin, '
           'nume, prenume, dataNasterii, imprumuturi, favorite'),

    blank(100),
    heading2('Descrierea claselor'),

    p('Clasa Carte', bold=True, size=22, after=80),
    normal('Clasa de bază abstractă definește interfața comună pentru toate tipurile de '
           'publicații. Conține metode virtuale pure afisareDetalii() și getTip() care '
           'trebuie implementate de fiecare clasă derivată. Câmpurile comune (titlu, autori, '
           'isbn etc.) sunt moștenite de toate subclasele.'),

    p('Clasa Biblioteca', bold=True, size=22, after=80),
    normal('Gestionează catalogul de cărți prin intermediul unui vector de pointeri unici '
           '(unique_ptr<Carte>). Utilizarea smart pointers garantează gestionarea automată '
           'a memoriei și elimină riscul de memory leak. Clasa oferă metode pentru '
           'adăugare, ștergere, căutare și afișare a cărților.'),

    p('Clasa Database', bold=True, size=22, after=80),
    normal('Responsabilă de persistența datelor. Serializează și deserializează obiectele '
           'din catalog în/din format JSON utilizând biblioteca nlohmann/json. '
           'Gestionează și fișierul de conturi (conturi.json), oferind funcții de '
           'autentificare și înregistrare.'),

    heading2('Secțiuni de cod deosebite'),

    p('1. Polimorfism și metode virtuale', bold=True, size=22, after=80),
    normal('Metodele virtuale getExtra1(), getExtra2(), getExtraInt() și getExtraBool() '
           'definite în clasa de bază Carte reprezintă un mecanism de polimorfism pentru '
           'serializare generică. Fiecare subclasă suprascrie aceste metode pentru a returna '
           'câmpurile sale specifice, permițând clasei Database să serializeze orice tip '
           'de carte fără cunoașterea tipului concret.'),

    p('2. Smart pointers și gestionarea memoriei', bold=True, size=22, after=80),
    normal('Catalogul utilizează vector<unique_ptr<Carte>>, asigurând ownership exclusiv '
           'și eliberarea automată a memoriei. La deserializare, factory method-ul din '
           'Database::incarcaDinFisier() creează instanțele corecte pe baza câmpului '
           '"tip" din JSON și le stochează ca unique_ptr<Carte>.'),

    p('3. API REST (Node.js)', bold=True, size=22, after=80),
    normal('Aplicația web expune un API REST complet prin Express.js. Endpoint-urile '
           'sunt organizate pe resurse: /api/carti (CRUD catalog), /api/imprumut '
           '(gestionare împrumuturi), /api/favorite, /api/recenzii, /api/amenzi și '
           '/api/users (administrare). Autentificarea se realizează prin sesiuni '
           'server-side (express-session) cu cookie-uri.'),

    p('4. Integrare API-uri externe', bold=True, size=22, after=80),
    normal('Aplicația web integrează două API-uri externe pentru îmbogățirea automată '
           'a datelor: Open Library API pentru cărți (imagini copertă, metadate) și '
           'OMDB API pentru adaptări cinematografice (postere, an lansare, regizor). '
           'Administratorul poate căuta și importa cărți direct din aceste surse.'),

    page_break(),
]

# ── CAPITOLUL III ─────────────────────────────────────────────────────────────
body_parts += [
    heading1('CAPITOLUL III'),
    heading1('Dezvoltări ulterioare'),
    blank(100),

    heading2('Funcționalități noi care ar fi necesare'),

    p('1. Sistem de rezervări', bold=True, size=22, after=80),
    normal('Implementarea unei cozi de așteptare pentru cărțile cu număr limitat de '
           'exemplare. Utilizatorii să poată rezerva o carte și să primească o notificare '
           'când aceasta devine disponibilă.'),

    p('2. Istoric citire și statistici personale', bold=True, size=22, after=80),
    normal('Istoricul complet al cărților citite de fiecare utilizator, cu statistici '
           'personale: număr de cărți citite pe lună, tipuri preferate, grafice de '
           'activitate. Integrarea unui sistem de "reading challenge" anual.'),

    p('3. Notificări prin email', bold=True, size=22, after=80),
    normal('Trimiterea automată de emailuri de reamintire cu 2-3 zile înainte de '
           'expirarea termenului de returnare, precum și notificări pentru amenzi '
           'acumulate sau rezervări disponibile.'),

    p('4. Grafice și rapoarte', bold=True, size=22, after=80),
    normal('Dashboard cu grafice interactive pentru administrator: evoluția împrumuturilor '
           'în timp, cele mai populare cărți, activitatea utilizatorilor, generare de '
           'rapoarte PDF.'),

    heading2('Modificări de cod sau de tehnologii de implementare'),

    p('1. Bază de date relațională', bold=True, size=22, after=80),
    normal('Înlocuirea fișierelor JSON cu o bază de date relațională (PostgreSQL sau SQLite) '
           'pentru scalabilitate și consistență mai bună a datelor, mai ales în scenarii '
           'cu acces concurent.'),

    p('2. Hashare parole', bold=True, size=22, after=80),
    normal('Implementarea hashării parolelor cu bcrypt în locul stocării în plaintext, '
           'crescând nivelul de securitate al aplicației. Adăugarea autentificării '
           'prin JWT (JSON Web Tokens) pentru API.'),

    p('3. Interfață grafică pentru C++', bold=True, size=22, after=80),
    normal('Înlocuirea interfeței TUI cu o interfață grafică folosind Qt sau wxWidgets, '
           'pentru o experiență mai modernă și accesibilă a aplicației desktop.'),

    p('4. Aplicație mobilă', bold=True, size=22, after=80),
    normal('Dezvoltarea unei aplicații mobile (React Native sau Flutter) care să consume '
           'același API REST, oferind acces la bibliotecă de pe dispozitive mobile.'),

    page_break(),
]

# ── BIBLIOGRAFIE ──────────────────────────────────────────────────────────────
body_parts += [
    heading1('BIBLIOGRAFIE'),
    blank(100),
    normal('[1] Documentație Laborator POO – Universitatea "Ștefan cel Mare", Suceava, 2025-2026'),
    normal('[2] Documentație Curs POO – Prof. Remus Prodan, 2025-2026'),
    normal('[3] Bjarne Stroustrup – "The C++ Programming Language", 4th Edition, Addison-Wesley, 2013'),
    normal('[4] nlohmann/json – C++ JSON library: https://github.com/nlohmann/json'),
    normal('[5] Express.js Documentation – https://expressjs.com'),
    normal('[6] Open Library API – https://openlibrary.org/developers/api'),
    normal('[7] OMDB API – https://www.omdbapi.com'),
    normal('[8] MDN Web Docs – JavaScript Reference: https://developer.mozilla.org'),
    normal('[9] ChatGPT (OpenAI) – utilizat ca asistent pentru debugging și documentație'),
]

body_xml = ''.join(body_parts)

# ── Build DOCX ────────────────────────────────────────────────────────────────
with zipfile.ZipFile(SRC, 'r') as zin:
    with zipfile.ZipFile(DST, 'w', zipfile.ZIP_DEFLATED) as zout:
        for item in zin.infolist():
            if item.filename == 'word/document.xml':
                old = zin.read(item.filename).decode('utf-8')
                # Preserve everything up to <w:body> and from </w:body> onwards
                pre  = old[:old.index('<w:body>') + len('<w:body>')]
                post = old[old.index('</w:body>'):]
                new_xml = pre + body_xml + post
                zout.writestr(item, new_xml.encode('utf-8'))
            else:
                zout.writestr(item, zin.read(item.filename))

print(f"Saved: {DST}")
