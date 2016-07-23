---
title: Optimización continua con Across Neighborhood Search
author: David Charte
theme: m
colortheme: metropolis
lang: spanish
polyglossia-lang.name: spanish
mainfont: "Fira Sans"
monofont: "Fira Mono"
graphics: true

classoption:
  - compress
header-includes:
  - "%\\metroset[sectionpage=none]"
  - \newcommand{\columnsbegin}{\begin{columns}}
  - \newcommand{\columnsend}{\end{columns}}
  - \definecolor{headbg}{RGB}{61, 96, 103}
  - \definecolor{highlight}{RGB}{95, 148, 159}
  - \definecolor{headfg}{RGB}{232, 239, 241}
  - \setbeamercolor{headtitle}{fg=headfg,bg=headbg}
  - \setbeamercolor{headnav}{fg=headfg,bg=headbg}
  - \setbeamercolor{section in head/foot}{fg=headfg,bg=headbg}
  - "\\defbeamertemplate*{headline}{miniframes theme no subsection}{
      \\begin{beamercolorbox}[ht=2.5ex,dp=1.125ex,
        leftskip=.3cm,rightskip=.3cm plus1fil]{headtitle}
        {\\usebeamerfont{title in head/foot}\\insertshorttitle}
        \\hfill
        \\leavevmode{\\usebeamerfont{author in head/foot}\\insertshortauthor}
      \\end{beamercolorbox}
      \\begin{beamercolorbox}[colsep=1.5pt]{upper separation line head}
      \\end{beamercolorbox}
      \\begin{beamercolorbox}{headnav}
        \\vskip2pt\\textsc{\\insertnavigation{\\paperwidth}}\\vskip2pt
      \\end{beamercolorbox}
      \\begin{beamercolorbox}[colsep=1.5pt]{lower separation line head}
      \\end{beamercolorbox}
    }"
  - "\\makeatletter\\renewcommand{\\@metropolis@frametitlestrut}{
      \\vphantom{ÁÉÍÓÚABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz}
    }\\makeatother"
  - "\\defbeamertemplate*{footline}{miniframes theme no subsection}{}"
  - \beamertemplatenavigationsymbolsempty
  - "%\\setbeameroption{show notes}"
  - \usepackage{algorithmic}
  - \usepackage{algorithm}
  - \floatname{algorithm}{Algoritmo}
  - \DeclareMathOperator*{\argmin}{arg\,min}
  - \DeclareMathOperator*{\argmax}{arg\,max}
---

# Introducción

## Optimización continua

Dada $f:D\subset\mathbb{R}^d \rightarrow \mathbb{R}$, encontrar $m\in D$ tal que $f(m)=\min\{f(x):x\in D\}$

\begin{figure}
\centering
\includegraphics[width=0.6\textwidth]{doc/img/schwefel.jpg}
\end{figure}

## Métodos clásicos

* **Exactos**: Derivación
* **Aproximados**: Métodos numéricos

## Métodos basados en Inteligencia Artificial

* Algoritmos **genéticos** para codificación real
* Algoritmos **meméticos** para codificación real
* Estrategias de evolución
* Enjambres de partículas (**PSO**)
* Evolución diferencial (**DE**)

# Across Neighborhood Search

## Cuestiones fundamentales

1. Cómo almacenar y actualizar una historia de **soluciones superiores** a los individuos
1. Cómo **aprender de dicha solución** superior para cada individuo
1. Cómo permitir que cada individuo infiera características de **varias soluciones** superiores

## Componentes de ANS

1. Colección de soluciones superiores
1. Búsqueda en el vecindario propio
1. Búsqueda a través de otros vecindarios

\begin{figure}[hbtp]
\centering
\includegraphics[width=0.4\textwidth]{doc/img/ans_neigh.png}
\caption{Ilustración de los vecindarios considerados en ANS\label{ans-neigh}}
\end{figure}

## Algoritmo (I)

\textbf{Entrada}: $p$ (tamaño población), $a$ (grado-ANS), $\sigma$ (desviación de la gaussiana)

Inicializar \textit{población} con $p$ soluciones aleatorias

\textit{superiores} $\gets$ \textit{población}

\textit{mejor} $\gets \argmax\limits_{x\in\textit{población}}f(x)$

## Algoritmo (II)

\small
\begin{algorithmic}
  \LOOP
    \FORALL{$n$ \textbf{in} $\{1,\dots $p$\}$}
      \STATE{$N\gets$ \{números de 1 a $d$ escogidos con probabilidad $a$\}}

      \FORALL{$i$ \textbf{in} $\{1,\dots d\}$}
        \IF{$i\in N$}
          \STATE{g $\gets$ \{número aleatorio de 1 a $p$ distinto de $n$\}}
          \STATE{$\textit{población}[n]_i=\textit{superiores}[g]_i+N(0,\sigma)\left\lvert \textit{población}[n]_i - \textit{superiores}[g]_i \right\rvert$}
        \ELSE
          \STATE{$\textit{población}[n]_i=\textit{superiores}[n]_i+N(0,\sigma)\left\lvert \textit{población}[n]_i - \textit{superiores}[n]_i \right\rvert$}
        \ENDIF
      \ENDFOR
      \STATE{registra mejoras en soluciones \textit{superiores} y \textit{mejor}}
    \ENDFOR
  \ENDLOOP
  \RETURN{\textit{mejor}}
\end{algorithmic}
\normalsize

## Experimentación preliminar: rendimiento

ANS [implementado en C++](https://github.com/fdavidcl/across-neighborhood-search), probado en las funciones test de CEC2014.

Buen rendimiento en:

* Funciones simples multimodales ($f4$ a $f16$)
* Funciones compuestas ($f23$ a $f30$)

Rendimiento pobre en:

* Funciones simples unimodales ($f1$ a $f3$)
* Funciones híbridas ($f17$ a $f22$)

## Experimentación preliminar: convergencia

\columnsbegin
\column{.5\textwidth}
\begin{figure}[hbtp]
\centering
\includegraphics[width=0.9\textwidth]{stats/out/ans_basic_f2d10.png}
\caption{Evolución de la diversidad en ANS sobre $f2$ en dimensión 10\label{div-ans1}}
\end{figure}
\column{.5\textwidth}
\begin{figure}[hbtp]
\centering
\includegraphics[width=0.9\textwidth]{stats/out/ans_basic_f5d10.png}
\caption{Evolución de la diversidad en ANS sobre $f5$ en dimensión 10\label{div-ans2}}
\end{figure}
\columnsend

# Propuestas de mejora

## Idea de mejora: ANS-Memory

Cuando un individuo mejora su solución superior, **sigue buscando en su vecindario** $\rightarrow$ desactiva la 3ª componente del algoritmo mientras mejore:

\begin{algorithmic}
  \IF{{\color{highlight}\textit{memoria}[n]}}
    \STATE{{\color{highlight}$N\gets\emptyset$}}
  \ELSE
    \STATE{$N\gets$ \{números de 1 a $d$ escogidos con probabilidad $a$\}}
  \ENDIF
\end{algorithmic}

## Búsqueda local: Hybrid-ANS

Cuando un individuo mejora a la mejor solución global, **se optimiza mediante búsqueda local**:

\begin{algorithmic}
  \IF{$f$(\textit{población}[n]) < $f$(\textit{mejor})}
    \IF{{\color{highlight}evaluaciones < $\beta\times$ max-evaluaciones}}
      \STATE{{\color{highlight}\textit{población}[n] $\gets$ optimiza(\textit{población}[n])}}
    \ENDIF
    \STATE{\textit{mejor} $\gets$ \textit{población}[n]}
  \ENDIF
\end{algorithmic}

### Búsquedas locales probadas

* Simplex (Nelder-Mead)
* Solis-Wets
* CMAES

## Autoadaptación: L-ANS y L-HANS

Se reduce el tamaño de la población linealmente, para aumentar la diversidad al inicio y favorecer la convergencia al final. Se utiliza el esquema LPSR de L-SHADE para ajustar el tamaño en cada generación:

$$p_{siguiente} \gets \frac{2 - p_{ini}}{\text{max-evaluaciones}}\times\text{evaluaciones}+p_{ini}$$

# Experimentación

## Rendimiento

\vspace{-2.8em}
\input{doc/d10.tex}

## Rendimiento: resumen

* **No hay una variante ganadora**: en dim. 10 HANS (CMAES) tiene el mejor ránking, en dim. 30 lo tiene ANS
* Frente a basados en DE y CMAES, es **más competitivo en dim. 30** que 10; y funciona bien en funciones multimodales ($f4$ a $f16$) y compuestas ($f23$ hasta $f30$)
* Frente a participantes de CEC2014, es **inferior a la mayoría** (falta de ajuste de parámetros y de rendimiento en funciones unimodales e híbridas)

## Convergencia

\columnsbegin
\column{.5\textwidth}
\begin{figure}[hbtp]
\centering
\includegraphics[width=0.9\textwidth]{stats/out/ans_basic_f3d10.png}
\caption{Evolución de la diversidad en ANS sobre $f3$ en dimensión 10\label{div-ans}}
\end{figure}
\column{.5\textwidth}
\begin{figure}[hbtp]
\centering
\includegraphics[width=0.9\textwidth]{stats/out/l_hans_cmaes_f3d10.png}
\caption{Evolución de la diversidad en L-HANS (CMAES) sobre $f3$ en dimensión 10\label{div-lhans}}
\end{figure}
\columnsend
