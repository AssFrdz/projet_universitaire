from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parent
OUT = ROOT / "figures"
OUT.mkdir(exist_ok=True)

BG = "#f6f7fb"
TEXT = "#162033"
MUTED = "#5f6b7a"
BLUE = "#2b6df3"
BLUE_SOFT = "#dfe9ff"
GREEN = "#13a37b"
GREEN_SOFT = "#d9f5ec"
ORANGE = "#ff9f43"
ORANGE_SOFT = "#fff0df"
PURPLE = "#6f42c1"
PURPLE_SOFT = "#eee6ff"
GRAY_BOX = "#ffffff"
GRAY_BORDER = "#d6dbe6"


def get_font(size: int, bold: bool = False):
    candidates = []
    if bold:
        candidates = [
            r"C:\Windows\Fonts\arialbd.ttf",
            r"C:\Windows\Fonts\calibrib.ttf",
            r"C:\Windows\Fonts\segoeuib.ttf",
        ]
    else:
        candidates = [
            r"C:\Windows\Fonts\arial.ttf",
            r"C:\Windows\Fonts\calibri.ttf",
            r"C:\Windows\Fonts\segoeui.ttf",
        ]
    for candidate in candidates:
        if Path(candidate).exists():
            return ImageFont.truetype(candidate, size=size)
    return ImageFont.load_default()


FONT_TITLE = get_font(44, bold=True)
FONT_SUBTITLE = get_font(24)
FONT_BOX = get_font(24, bold=True)
FONT_BODY = get_font(20)
FONT_SMALL = get_font(17)
FONT_MONO = get_font(18)


def canvas(size=(1600, 900)):
    return Image.new("RGB", size, BG), ImageDraw.Draw(Image.new("RGB", size, BG))


def new_canvas(size=(1600, 900)):
    img = Image.new("RGB", size, BG)
    return img, ImageDraw.Draw(img)


def rounded_box(draw, xy, fill, outline=GRAY_BORDER, radius=24, width=3):
    draw.rounded_rectangle(xy, radius=radius, fill=fill, outline=outline, width=width)


def shadowed_box(draw, xy, fill, outline=GRAY_BORDER, radius=24):
    x1, y1, x2, y2 = xy
    draw.rounded_rectangle((x1 + 8, y1 + 10, x2 + 8, y2 + 10), radius=radius, fill="#dfe4ef")
    rounded_box(draw, xy, fill=fill, outline=outline, radius=radius)


def center_text(draw, x1, y1, x2, y2, title, lines, title_fill=TEXT):
    draw.text((x1 + 24, y1 + 18), title, font=FONT_BOX, fill=title_fill)
    text_y = y1 + 62
    for line in lines:
        draw.text((x1 + 24, text_y), line, font=FONT_BODY, fill=TEXT)
        text_y += 30


def section_chip(draw, xy, text, fill, text_fill="#ffffff"):
    x1, y1, x2, y2 = xy
    draw.rounded_rectangle(xy, radius=18, fill=fill)
    bbox = draw.textbbox((0, 0), text, font=FONT_SMALL)
    tw = bbox[2] - bbox[0]
    th = bbox[3] - bbox[1]
    draw.text((x1 + (x2 - x1 - tw) / 2, y1 + (y2 - y1 - th) / 2 - 1), text, font=FONT_SMALL, fill=text_fill)


def arrow(draw, start, end, color=BLUE, width=8, head=18):
    x1, y1 = start
    x2, y2 = end
    draw.line((x1, y1, x2, y2), fill=color, width=width)
    if abs(x2 - x1) >= abs(y2 - y1):
        if x2 >= x1:
            pts = [(x2, y2), (x2 - head, y2 - head // 2), (x2 - head, y2 + head // 2)]
        else:
            pts = [(x2, y2), (x2 + head, y2 - head // 2), (x2 + head, y2 + head // 2)]
    else:
        if y2 >= y1:
            pts = [(x2, y2), (x2 - head // 2, y2 - head), (x2 + head // 2, y2 - head)]
        else:
            pts = [(x2, y2), (x2 - head // 2, y2 + head), (x2 + head // 2, y2 + head)]
    draw.polygon(pts, fill=color)


def title_block(draw, title, subtitle):
    draw.text((80, 54), title, font=FONT_TITLE, fill=TEXT)
    draw.text((82, 112), subtitle, font=FONT_SUBTITLE, fill=MUTED)


def save(img, name):
    img.save(OUT / name)


def workflow_base():
    img, draw = new_canvas()
    title_block(
        draw,
        "Workflow de base de la documentation Marmote",
        "Les exemples C++ et les notebooks Python sont préparés séparément avant la génération Sphinx.",
    )

    section_chip(draw, (80, 180, 240, 222), "Entrées", BLUE)
    section_chip(draw, (615, 180, 825, 222), "Traitements", GREEN)
    section_chip(draw, (1170, 180, 1360, 222), "Sorties", ORANGE)

    shadowed_box(draw, (80, 260, 440, 410), BLUE_SOFT, outline="#c6d7ff")
    center_text(draw, 80, 260, 440, 410, "xpl / build", ["Sources des exemples C++", "Compilation avec CMake", "Exécutables dans build/bin"])

    shadowed_box(draw, (80, 470, 440, 620), PURPLE_SOFT, outline="#d8c8ff")
    center_text(draw, 80, 470, 440, 620, "pytutos", ["Notebooks Python", "Images et contenu pédagogique", "Documentation par notebooks"])

    shadowed_box(draw, (580, 250, 950, 430), GREEN_SOFT, outline="#b9e8d8")
    center_text(
        draw,
        580,
        250,
        950,
        430,
        "doc_copyExamples.py",
        ["Exécute les exemples compilés", "Génère les fichiers .res et .cmd", "Copie les sources .cpp dans media", "Met à jour le CMakeLists.txt"],
    )

    shadowed_box(draw, (580, 470, 950, 650), GREEN_SOFT, outline="#b9e8d8")
    center_text(
        draw,
        580,
        470,
        950,
        650,
        "doc_generateArchives.py",
        ["Copie les notebooks", "Prépare les variantes Google Colab", "Crée les archives de notebooks", "Réécrit les références d'images"],
    )

    shadowed_box(draw, (1110, 245, 1510, 420), ORANGE_SOFT, outline="#ffd6a8")
    center_text(
        draw,
        1110,
        245,
        1510,
        420,
        "Ressources de documentation",
        ["doc/html/source/media", ".cpp + .res + .cmd files", "all_examples.zip", "Updated example CMakeLists.txt"],
        title_fill="#9c5600",
    )

    shadowed_box(draw, (1110, 470, 1510, 675), ORANGE_SOFT, outline="#ffd6a8")
    center_text(
        draw,
        1110,
        470,
        1510,
        675,
        "Ressources notebooks",
        ["pytutos_colab", "all_notebooks.zip", "all_notebooks_colab.zip", "all_pythons.zip"],
        title_fill="#9c5600",
    )

    shadowed_box(draw, (1110, 720, 1510, 835), GRAY_BOX, outline=GRAY_BORDER)
    center_text(draw, 1110, 720, 1510, 835, "Étape finale", ["Sphinx génère la documentation HTML"], title_fill=TEXT)

    arrow(draw, (440, 335), (580, 335), BLUE)
    arrow(draw, (440, 545), (580, 555), PURPLE)
    arrow(draw, (950, 335), (1110, 332), GREEN)
    arrow(draw, (950, 555), (1110, 575), GREEN)
    arrow(draw, (1310, 675), (1310, 720), ORANGE)

    save(img, "workflow_base.png")


def workflow_target():
    img, draw = new_canvas()
    title_block(
        draw,
        "Workflow cible avec notebooks C++",
        "Un script unifié prépare les notebooks Python, les notebooks C++, les archives et les ressources d'exemple.",
    )

    section_chip(draw, (80, 175, 240, 217), "Sources", BLUE)
    section_chip(draw, (670, 175, 930, 217), "Pilotage unifié", PURPLE)
    section_chip(draw, (1230, 175, 1450, 217), "Livrables", GREEN)

    shadowed_box(draw, (80, 250, 420, 390), BLUE_SOFT, outline="#c6d7ff")
    center_text(draw, 80, 250, 420, 390, "xpl", ["Exemples C++ officiels", "Comportement de référence", "Sorties d'exemple"])

    shadowed_box(draw, (80, 430, 420, 570), BLUE_SOFT, outline="#c6d7ff")
    center_text(draw, 80, 430, 420, 570, "pytutos", ["Leçons Python de référence", "Structure pédagogique", "Texte et figures"])

    shadowed_box(draw, (80, 610, 420, 750), BLUE_SOFT, outline="#c6d7ff")
    center_text(draw, 80, 610, 420, 750, "cpptutos", ["Notebooks C++", "Cellules de configuration Xeus-cling", "Fichiers .cpp exportables"])

    shadowed_box(draw, (540, 305, 980, 690), PURPLE_SOFT, outline="#d8c8ff")
    center_text(
        draw,
        540,
        305,
        980,
        690,
        "total.py",
        [
            "Vérifie le répertoire de travail",
            "Lance doc_generateArchives.py",
            "Lance doc_copyExamples.py",
            "Crée les archives Colab / notebooks / C++",
            "Maintient les fichiers CMake cohérents",
            "Prépare les ressources pour Sphinx",
        ],
        title_fill="#4f2e8d",
    )

    section_chip(draw, (620, 258, 900, 294), "Point d'entrée unique", PURPLE, text_fill="#ffffff")

    shadowed_box(draw, (1120, 245, 1520, 385), GREEN_SOFT, outline="#b9e8d8")
    center_text(draw, 1120, 245, 1520, 385, "Paquet exemples", ["media/ + all_examples.zip", "CMakeLists.txt mis à jour"], title_fill="#0c6b51")

    shadowed_box(draw, (1120, 425, 1520, 565), GREEN_SOFT, outline="#b9e8d8")
    center_text(draw, 1120, 425, 1520, 565, "Paquets notebooks", ["all_notebooks.zip", "all_notebooks_colab.zip", "all_pythons.zip"], title_fill="#0c6b51")

    shadowed_box(draw, (1120, 605, 1520, 785), GREEN_SOFT, outline="#b9e8d8")
    center_text(draw, 1120, 605, 1520, 785, "Pages de documentation", ["Sortie HTML Sphinx", "Tutoriels Python + C++", "Ressources téléchargeables cohérentes"], title_fill="#0c6b51")

    arrow(draw, (420, 320), (540, 390), BLUE)
    arrow(draw, (420, 500), (540, 500), BLUE)
    arrow(draw, (420, 680), (540, 610), BLUE)
    arrow(draw, (980, 350), (1120, 315), PURPLE)
    arrow(draw, (980, 500), (1120, 495), PURPLE)
    arrow(draw, (980, 645), (1120, 695), PURPLE)

    save(img, "workflow_cible.png")


def sphinx_cpp_api():
    img, draw = new_canvas(size=(1600, 1000))
    title_block(
        draw,
        "Exemple de page Sphinx pour l'API C++",
        "Maquette illustrative du site de documentation Marmote utilisé dans le rapport.",
    )

    rounded_box(draw, (70, 170, 1530, 930), fill="#ffffff", outline=GRAY_BORDER, radius=28, width=3)
    rounded_box(draw, (70, 170, 1530, 230), fill="#eef3fb", outline="#eef3fb", radius=28, width=0)
    draw.rectangle((70, 210, 1530, 230), fill="#eef3fb")

    for i, color in enumerate(["#ff5f57", "#febc2e", "#28c840"]):
        draw.ellipse((98 + i * 26, 191, 116 + i * 26, 209), fill=color)

    draw.text((170, 188), "https://marmote.gitlabpages.inria.fr/marmote/cpp_api.html", font=FONT_SMALL, fill=MUTED)

    rounded_box(draw, (100, 260, 390, 885), fill="#f7f9fd", outline="#e4e9f2", radius=18, width=2)
    draw.text((128, 288), "Navigation", font=FONT_BOX, fill=TEXT)
    sidebar_items = [
        "Présentation",
        "Compilation",
        "C++ API",
        "Lesson 1",
        "Lesson 2",
        "Lesson 3",
        "Lesson 4",
        "MDP Lesson 1",
        "Téléchargements",
    ]
    y = 340
    for item in sidebar_items:
        fill = BLUE if item == "C++ API" else "#ffffff"
        text_fill = "#ffffff" if item == "C++ API" else TEXT
        rounded_box(draw, (120, y, 360, y + 46), fill=fill, outline=fill if item == "C++ API" else "#e4e9f2", radius=14, width=2)
        draw.text((138, y + 11), item, font=FONT_BODY, fill=text_fill)
        y += 58

    draw.text((450, 280), "Documentation Marmote", font=FONT_SMALL, fill=BLUE)
    draw.text((450, 320), "C++ API", font=FONT_TITLE, fill=TEXT)
    draw.text((452, 382), "Pages de référence et leçons à base de notebooks pour l'interface C++.", font=FONT_SUBTITLE, fill=MUTED)

    rounded_box(draw, (450, 455, 980, 630), fill=BLUE_SOFT, outline="#cad9ff", radius=20, width=2)
    draw.text((480, 485), "Sections disponibles", font=FONT_BOX, fill=TEXT)
    info_lines = [
        "• Instructions de compilation et d'installation",
        "• Leçons élémentaires sur les chaînes de Markov",
        "• Leçons et exemples MDP",
        "• Archives téléchargeables (.zip, .cpp, notebooks)",
    ]
    yy = 535
    for line in info_lines:
        draw.text((485, yy), line, font=FONT_BODY, fill=TEXT)
        yy += 32

    rounded_box(draw, (1020, 455, 1460, 760), fill="#0f172a", outline="#0f172a", radius=20, width=2)
    draw.text((1050, 485), "Exemple de code", font=FONT_BOX, fill="#ffffff")
    code = [
        "#include <marmoteMarkovChain/marmoteMarkovChain.h>",
        "",
        "SparseMatrix* P = new SparseMatrix(3);",
        "DiscreteDistribution* d0 = new DiracDistribution(0);",
        "MarkovChain* mc = new MarkovChain(P);",
        "mc->TransientDistribution(d0, 10.0)->Write();",
    ]
    yy = 535
    for line in code:
        draw.text((1050, yy), line, font=FONT_MONO, fill="#d8e1ff")
        yy += 34

    rounded_box(draw, (450, 680, 980, 830), fill=GREEN_SOFT, outline="#b9e8d8", radius=20, width=2)
    draw.text((480, 712), "Objectif documentaire", font=FONT_BOX, fill="#0c6b51")
    draw.text((482, 760), "Exposer des tutoriels Python et C++ cohérents via Sphinx.", font=FONT_BODY, fill=TEXT)

    save(img, "sphinx_cpp_api.png")


def tag_cells():
    img, draw = new_canvas(size=(1600, 980))
    title_block(
        draw,
        "Ajout de tags dans un notebook Jupyter",
        "Vue illustrative des métadonnées utilisée pour expliquer le masquage des cellules techniques.",
    )

    rounded_box(draw, (70, 170, 1530, 910), fill="#ffffff", outline=GRAY_BORDER, radius=28, width=3)
    rounded_box(draw, (70, 170, 1530, 230), fill="#f1f4fb", outline="#f1f4fb", radius=28, width=0)
    draw.rectangle((70, 210, 1530, 230), fill="#f1f4fb")
    draw.text((110, 188), "JupyterLab", font=FONT_BOX, fill=TEXT)
    draw.text((300, 188), "MDP_Lesson1.ipynb", font=FONT_SMALL, fill=MUTED)

    rounded_box(draw, (100, 260, 940, 840), fill="#fafbfe", outline="#e6ebf4", radius=22, width=2)
    section_chip(draw, (122, 284, 260, 320), "Cellule code", BLUE)
    draw.text((126, 342), "#pragma cling add_include_path(\"/home/assia/.../include\")", font=FONT_MONO, fill=TEXT)
    draw.text((126, 380), "#pragma cling add_library_path(\"/home/assia/.../lib\")", font=FONT_MONO, fill=TEXT)
    draw.text((126, 418), "#pragma cling load(\"libmarmoteCore.so\")", font=FONT_MONO, fill=TEXT)
    draw.text((126, 456), "#pragma cling load(\"libmarmoteMarkovChain.so\")", font=FONT_MONO, fill=TEXT)

    rounded_box(draw, (122, 520, 910, 790), fill="#ffffff", outline="#dfe5ef", radius=18, width=2)
    draw.text((150, 548), "But", font=FONT_BOX, fill=TEXT)
    bullet_lines = [
        "• Conserver la configuration Xeus-cling dans le notebook.",
        "• Masquer la cellule technique dans la documentation Sphinx.",
        "• Préserver un déroulé pédagogique plus lisible.",
    ]
    yy = 600
    for line in bullet_lines:
        draw.text((154, yy), line, font=FONT_BODY, fill=TEXT)
        yy += 36

    rounded_box(draw, (1000, 260, 1480, 840), fill="#0f172a", outline="#0f172a", radius=22, width=2)
    draw.text((1032, 292), "Éditeur de métadonnées", font=FONT_BOX, fill="#ffffff")
    json_lines = [
        "{",
        '  "tags": [',
        '    "hide-cell",',
        '    "nbsphinx-hidden"',
        "  ],",
        '  "jupyter": {',
        '    "source_hidden": true',
        "  }",
        "}",
    ]
    yy = 360
    for line in json_lines:
        fill = "#88c0ff" if "tags" in line or "jupyter" in line else "#e7eefc"
        if "hide-cell" in line or "nbsphinx-hidden" in line:
            fill = "#9ef0c5"
        draw.text((1040, yy), line, font=FONT_MONO, fill=fill)
        yy += 40

    rounded_box(draw, (1030, 660, 1450, 790), fill=ORANGE_SOFT, outline="#ffd6a8", radius=18, width=2)
    draw.text((1060, 694), "Effet attendu dans Sphinx", font=FONT_BOX, fill="#9c5600")
    draw.text((1060, 734), "La cellule technique est masquée", font=FONT_SMALL, fill=TEXT)
    draw.text((1060, 762), "dans la page HTML finale.", font=FONT_SMALL, fill=TEXT)

    save(img, "tag_cells.png")


def main():
    workflow_base()
    workflow_target()
    sphinx_cpp_api()
    tag_cells()
    print("Generated figures in", OUT)


if __name__ == "__main__":
    main()
