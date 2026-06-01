# Rapport LaTeX

Ce dossier contient une version LaTeX exploitable du rapport à partir du texte fourni.

## Fichiers

- `main.tex` : document principal
- `pagedegarde.tex` : macros de page de garde utilisées par le document
- `figures/` : dossier prévu pour les figures locales

## Compilation

Depuis la racine du dépôt :

```powershell
cd rapport-latex
pdflatex main.tex
```

ou avec `latexmk` :

```powershell
cd rapport-latex
latexmk -pdf main.tex
```

## Figures

Le document essaie d'abord de charger les images depuis `figures/`, puis depuis
les répertoires déjà présents dans le dépôt. Si une image manque encore, une
boîte de remplacement est affichée à la place pour éviter de bloquer la compilation.
