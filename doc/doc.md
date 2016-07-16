---
title: "Práctica final de Metaheurísticas"
subtitle: "Optimización continua con Across Neighborhood Search"
author: "Francisco David Charte Luque <<fdavidcl@correo.ugr.es>>"
toc: yes
fontsize: 11pt
geometry: "a4paper, top=2.2cm, bottom=2.2cm, left=2.6cm, right=2.6cm"
bibliography: doc/references.bib
csl: doc/ieee.csl
numbersections: yes
header-includes:
  - \usepackage[spanish, es-tabla]{babel}
  - \usepackage{graphicx}
  - \usepackage[usenames,dvipsnames,svgnames,table]{xcolor}
  - \usepackage{multirow}
  - \usepackage{amsopn}
  - \usepackage{algorithmic}
  - \usepackage{algorithm}
  - \floatname{algorithm}{Algoritmo}
  - \renewcommand{\algorithmicrequire}{\textbf{Input:}}
  - \renewcommand{\algorithmicensure}{\textbf{Output:}}
  - \usepackage{pdflscape}
  - \DeclareMathOperator*{\argmin}{arg\,min}
  - \DeclareMathOperator*{\argmax}{arg\,max}
---

\clearpage

# Introducción

Un problema de optimización continua es aquel donde se trata de encontrar el valor que hace mínima una función real de dominio continuo:
$$\mbox{Dada }f:D\subset\mathbb{R}^d \rightarrow \mathbb{R}\mbox{, encontrar }m\in D\mbox{ tal que }f(m)=\min\{f(x):x\in D\}~.$$

Aunque en funciones muy sencillas, con buenas propiedades analíticas, esto se puede conseguir de forma exacta mediante derivación, no es el caso usual en la realidad. Por ello, se puede recurrir a técnicas numéricas, pero pueden resultar poco eficientes y problemáticas para funciones con varios óptimos locales o falta de derivabilidad. Las técnicas de Inteligencia Artificial permiten explorar de forma eficiente el dominio y llegar a soluciones aproximadas de calidad en un tiempo razonable.

Los problemas de funciones de dominio continuo con varios óptimos y no derivabilidad son comunes en áreas como las telecomunicaciones [@tnep], sistemas de energía [@psoelectric] y en estudio de trayectorias en Física [@psotraj]. Se trata, por tanto, de un tema con múltiples aplicaciones en la actualidad.

Muchas de las técnicas más recientes orientadas a optimización continua son algoritmos de búsqueda basada en poblaciones, es decir, modelos que toman una población inicial y buscan mejoras mediante diferentes operadores que permiten combinar y alterar soluciones. Algunas de las utilizadas clásicamente son algoritmos genéticos para codificación real [@herrera1998] y los correspondientes meméticos, estrategias de evolución [@beyer2002], enjambres de partículas (*Particle Swarm Optimization*, PSO) [@kennedy2011] y algoritmos basados en evolución diferencial (*Differential Evolution*, DE) [@storn1997].

Sin embargo, en los últimos años han surgido nuevas estrategias para optimización continua, muchas de ellas inspiradas en la biología, que tratan de ofrecer un comportamiento ligeramente distinto pero que permita llegar a resultados competitivos. En este trabajo se analiza una de estas nuevas técnicas, denominada *Across Neighborhood Search* (ANS), se compara con varias de las heurísticas competitivas del ámbito y se exploran diversas posibles mejoras que podrían repercutir positivamente en su comportamiento.

# *Across Neighborhood Search*

ANS es una técnica propuesta en [@wu2016ans] y que tiene como objetivo aportar soluciones sencillas a las tres cuestiones que el autor considera fundamentales en un algoritmo basado en poblaciones:

1. Cómo almacenar y actualizar una historia de **soluciones superiores** a los individuos
1. Cómo **aprender de dicha solución** superior para cada individuo
1. Cómo permitir que cada individuo infiera características de **varias soluciones** superiores

El algoritmo resultante trata de construir componentes que resuelvan cada uno de estos puntos sin añadir demasiada complejidad al conjunto. De esta forma, por un lado se obtiene un método sencillo y atractivo para optimización continua, fácil de entender, y con potencial para mejora mediante diversas extensiones; por ejemplo, la hibridación con búsquedas locales o la adaptación de parámetros automática.

ANS es una técnica, en esencia similar a PSO, donde un conjunto de individuos realiza una búsqueda guiada por una memoria de buenas soluciones anteriores y recorre distintas posiciones en los vecindarios de las mismas. La versión básica del algoritmo únicamente cuenta con **tres parámetros ajustables**: el tamaño de la población, el grado de aplicación de la búsqueda a través de vecindarios y la desviación de la distribución gaussiana utilizada para moverse en un vecindario.

## Descripción del algoritmo

El algoritmo de búsqueda se basa en tres componentes: la colección de soluciones superiores, la búsqueda de cada individuo en el vecindario de su solución superior y la búsqueda a través de otros vecindarios

### Colección de soluciones superiores

ANS mantiene una colección de soluciones denominadas superiores. En la versión básica del algoritmo presentada en [@wu2016ans], la colección está formada por la mejor solución encontrada por cada individuo hasta el momento. De esta forma, la colección tiene el mismo cardinal que la población. Sin embargo, se podrían construir variantes de ANS donde el tamaño de dicha colección sea distinto, o incluso varíe a lo largo de la ejecución.

### Búsqueda en el vecindario propio

Puesto que a cada individuo le corresponde una solución superior, puede explorar en un entorno de la misma en una o varias dimensiones a la vez. ANS trata cada dimensión por separado al decidir el movimiento del individuo. Si consideramos que el individuo $x$ se moverá en la $i$-ésima dirección ($1\leq i\leq d$) en el entorno de su solución superior $r$, notando un valor tomado de la distribución normal con desviación $\sigma$ como $N(0,\sigma)$, se tiene el cálculo siguiente:
$$x_i\gets r_i+N(0,\sigma)\left\lvert x_i - r_i\right\rvert$$

### Búsqueda a través de otros vecindarios

En ANS se asume que es conveniente que cada individuo pueda aprender de todas las soluciones superiores y no únicamente de su propia solución superior. Así, puesto que en cada movimiento se tratan las dimensiones por separado, **algunas de ellas se escogen para moverse alrededor de soluciones superiores** que no sean la propia del individuo. La probabilidad de que una dimensión sea escogida se denomina grado de búsqueda a través de vecindarios, o grado-ANS.

Esta tercera componente es, por tanto, la que marca la diferencia con otros algoritmos basados en poblaciones. De esta forma, un individuo puede dar "saltos" en algunas de las dimensiones del dominio y seguir explorando su propio vecindario en otras dimensiones. Se ilustra este comportamiento en la Figura \ref{ans-neigh}: en morado y verde oscuro se muestran dos supuestas soluciones superiores de la colección, con sus correspondientes vecindarios en color claro. Puesto que un individuo puede tomar valores en distintos vecindarios según la dimensión, el círculo azul denota un posible rango de búsqueda para un individuo cuya solución superior sea o bien la morada o bien la verde.

\begin{figure}[hbtp]
\centering
\includegraphics[width=0.6\textwidth]{doc/img/ans_neigh.png}
\caption{Ilustración de los vecindarios considerados en ANS\label{ans-neigh}}
\end{figure}

Como consecuencia de esta estrategia, ANS introduce un componente de exploración bajo el criterio de aprendizaje de las mejores soluciones obtenidas, asumiendo que inferir algunas características de varias de las mismas es conveniente en la búsqueda del óptimo.

### Pseudocódigo

El algoritmo \ref{algo-ans} describe el funcionamiento de ANS.
\input{doc/algo_ans.tex}

## Implementación

La implementación se ha realizado en el lenguaje C++ siguiendo minuciosamente la descripción del autor original. Se ha estructurado en una clase `ANSBase` que sirve de ancestro al resto de variantes de ANS. El software está disponible para su uso y libre distribución bajo los términos de la licencia GPLv3[^gpl] en \url{https://github.com/fdavidcl/across-neighborhood-search}.

[^gpl]: El texto de la licencia está disponible como parte del software, y en \url{https://www.gnu.org/licenses/gpl-3.0.html}.

## Experimentación preliminar

Se han realizado algunos experimentos previos al desarrollo de posibles mejoras, con el propósito de identificar los puntos fuertes y los problemáticos de ANS. Para ello, se ha ejecutado el algoritmo sobre las funciones de la competición CEC2014 [@liang2013] ajustando los parámetros a diferentes valores para observar también la variación en el rendimiento.

Como resultado, se intuyen los siguientes problemas:

* No tiene buen comportamiento en problemas unimodales: las funciones $f1$, $f2$ y $f3$ del paquete de funciones test tienen un único mínimo, y ANS no es capaz de acercarse. Los resultados concretos se pueden comprobar en la tablas \ref{d10} y \ref{d30} para dimensiones 10 y 30, respectivamente.
* Tiene un problema de diversidad: en algunas funciones la distancia entre los individuos se reduce muy rápidamente, y en otras se mantiene o aumenta con el paso de las generaciones. Este comportamiento se observa en las figuras \ref{div-ans1} y \ref{div-ans2}.

\begin{figure}[hbtp]
\centering
\includegraphics[width=0.6\textwidth]{stats/out/ans_basic_f2d10.png}
\caption{Evolución de la diversidad en ANS sobre $f2$ en dimensión 10\label{div-ans1}}
\end{figure}
\begin{figure}[hbtp]
\centering
\includegraphics[width=0.6\textwidth]{stats/out/ans_basic_f5d10.png}
\caption{Evolución de la diversidad en ANS sobre $f5$ en dimensión 10\label{div-ans2}}
\end{figure}

ANS aparenta tener, sin embargo, muy buen comportamiento en varias de las funciones simples multimodales ($f4$ a $f16$) y en todas las funciones compuestas ($f23$ a $f30$), acercándose a niveles competitivos con los algoritmos basados en DE en bastantes de ellas. Además, no se aprecia una diferencia significativa al variar ligeramente los parámetros del algoritmo, por lo que para el resto de la experimentación se han ajustado a los recomendados por el autor.

# Propuestas de mejora

## Memoria individual de mejora previa

Uno de los aspectos en los que el comportamiento de ANS puede resultar extraño es en que, a cada generación, los individuos **saltan a posiciones fuera de su vecindario** anterior, a causa de la búsqueda a través de otros vecindarios. Esto puede impedir que un individuo encuentre mejoras explorando su propio vecindario antes de saltar a otro entorno. Por esta razón, se propone una variante del algoritmo, **ANS-Memory**, que elimina esta restricción, permitiendo que los individuos memoricen el hecho de haber mejorado el valor de su solución en la iteración anterior, de forma que puedan seguir buscando localmente si se da el caso. El algoritmo \ref{algo-ansm} describe en detalle el funcionamiento de esta variante.

\input{doc/algo_ansm.tex}

## Hibridación con búsqueda local

Otra vía para la mejora en la búsqueda del vecindario de buenas soluciones es la hibridación con una técnica de búsqueda local. El autor de ANS cuenta con una propuesta anterior de hibridación de PSO con colección de soluciones superiores y búsqueda local [@wu2014superior], por lo que se ha tomado como inspiración para construir la variante híbrida de ANS, **Hybrid-ANS** (HANS).

La idea fundamental es optimizar siempre la mejor solución encontrada, de forma que los individuos tomen algunos valores de ella en las siguientes generaciones y se muevan a posiciones de mayor calidad. Para ello, cada vez que se mejora la solución global, se optimiza la nueva solución. Este comportamiento se plasma en el algoritmo \ref{algo-hans}.

\input{doc/algo_hans.tex}

En la implementación se han utilizado varias técnicas de búsqueda local distintas, concretamente el método de Nelder-Mead (o *downhill simplex*) [@dennis1987], la técnica de Solis-Wets [@solis1981] y el potente algoritmo CMAES [@hansen2003].

## Autoadaptación de parámetros

Como se ha observado en la experimentación, un problema grave de ANS es la inestabilidad de la diversidad de población: según la función a optimizar, ANS puede presentar una convergencia muy fuerte o una total falta de ella. Por esto, podría convenir un ajuste dinámico de algún parámetro que facilite que la diversidad se mantenga alta durante más generaciones al principio, permitiendo que haya convergencia hacia el final de la ejecución.

La opción que se ha explorado es la **variación del tamaño de la población** con el paso de las generaciones. Puesto que construir un mecanismo de adaptación complejo requeriría un amplio estudio previo acerca del efecto de la alteración del tamaño de la población en el rendimiento de ANS, se ha optado por un decremento lineal tomado del algoritmo L-SHADE [@tanabe2014]. Se trata de un pequeño método que se ejecuta al final de cada generación, se encuentra descrito en el algoritmo \ref{algo-lans}.

\input{doc/algo_lans.tex}

Este mecanismo se ha añadido tanto al ANS básico como a Hybrid-ANS, con el objetivo de estudiar si las mejoras acumuladas mediante la búsqueda local y la variación de la población tienen un mayor impacto en el rendimiento. Los algoritmos resultantes se han denominado **L-ANS** y **L-HANS**, respectivamente.

# Experimentación

## Casos del problema y parámetros

Para estudiar el funcionamiento de ANS y las variantes desarrolladas, se han utilizado las funciones test de la competición CEC2014 [@liang2013] en dimensión 10 y 30. Dichas funciones se agrupan en tres categorías:

* Funciones simples unimodales: $f1$ a $f3$
* Funciones simples multimodales: $f4$ a $f16$
* Funciones híbridas: $f17$ a $f22$
* Funciones compuestas: $f23$ a $f30$

Se han realizado 25 ejecuciones de cada algoritmo sobre todas las funciones y se han promediado los resultados. Además, para situarse en igualdad de condiciones con los algoritmos comparados se ha establecido un máximo de 100000 evaluaciones de la función en dimensión 10, y 300000 en dimensión 30.

Los parámetros que se han fijado para ANS y sus variantes son:

* Tamaño de la población para ANS, ANS-M y HANS: 20 (recomendado por el autor)
* Tamaño inicial de la población en L-ANS: 100 y 200 (en las tablas de resultados se denominan *100l_ans* y *200l_ans* respectivamente)
* Tamaño inicial de la población en L-HANS: 100
* Grado-ANS: 10 (el autor afirma que el ajuste óptimo de este parámetro puede variar con el problema, se necesitaría profundizar en el estudio para conocer un mejor valor para cada problema)
* Desviación de la distribución gaussiana: 0.5 (recomendado por el autor)
* Razón máxima de evaluaciones consumidas por búsqueda local: 40% (la búsqueda local no siempre consumirá el 40% de las evaluaciones, pero es el límite máximo que puede llegar a consumir)

ANS y sus variantes se han comparado con resultados de una batería de algoritmos basados en DE y con variantes de CMAES del estado del arte: NBIPOP-ACMA-ES e iCMAES-ILS.

## Reproducibilidad

Las mediciones tomadas se han realizado tomando semillas aleatorias a partir del tiempo, como se especifica en las condiciones de la competición CEC2014. Por tanto, no se pueden reproducir con exactitud los resultados, pero las tablas que figuran en la sección de resultados se han compuesto mediante un guion publicado en el repositorio junto al resto del software. En conjunto, este documento es reproducible mediante los siguientes comandos desde la raíz del repositorio:

~~~bash
# Compila el programa
make
# Realiza las ejecuciones y guarda los resultados
bin/run ans_basic ans_memory hans_simplex hans_cmaes hans_sw \
  l_ans l_hans_simplex l_hans_cmaes l_hans_sw
# Procesa los resultados y genera tablas y gráficos
stats/stats.R
# Compone el documento actual
make doc
~~~

Son necesarios los intérpretes de los lenguajes Ruby y R, un compilador de C++ y el programa Pandoc para ejecutar los comandos previos.

\begin{landscape}
  \subsection{Resultados}

  A continuación se muestran los resultados de las ejecuciones realizadas. Se destaca en negrita el mejor resultado para cada función, y la última fila (en las primeras dos tablas) o columna (en las demás) indica el ránking promedio de cada variante. Las tablas \ref{complete10a}, \ref{complete10b}, \ref{complete30a} y \ref{complete30b} comparan las variantes de ANS con técnicas basadas en DE y CMAES, y las \ref{cec10a}, \ref{cec10b}, \ref{cec30a} y \ref{cec30b} las sitúan en la competición CEC2014.

  \input{doc/d10.tex}

  \input{doc/d30.tex}

  \input{doc/c10.tex}

  \input{doc/c30.tex}

  \input{doc/cec10.tex}

  \input{doc/cec30.tex}
  \clearpage
\end{landscape}

## Análisis

Lo primero que se deduce a la vista de los datos recogidos es que ninguna de las variantes se impone decisivamente sobre las demás, y ni siquiera se observa que las variantes mejoren claramente a la versión básica de ANS. En dimensión 10, la mejor variante de ANS según el ranking medio es la hibridación con CMAES, seguida de cerca por la original. En dimensión 30, la original tiene el ránking medio más alto y le siguen las variantes híbridas.

El test de Friedman otorga unos p-values de 0.07234 a los datos tomados para dimensión 10 y $2.2\times 10^{-16}$ para dimensión 30, luego podemos rechazar, con una confianza del 92% en el primer caso y con casi total seguridad en el segundo, la hipótesis de que todas las variantes de ANS se comporten igual y las diferencias se deban a errores.

Frente al resto de algoritmos basados en DE y CMAES, la situación de ANS y sus variantes es la que ya se intuía durante la experimentación preliminar. En dimensión 10, ANS no es capaz de acercarse totalmente al rendimiento de los demás, aunque sí que es razonablemente competitivo en la mayoría de las funciones multimodales ($f4$ a $f16$) y compuestas ($f23$ hasta $f30$). Mientras tanto, en dimensión 30, ANS sí es capaz de obtener algunos resultados muy similares o mejores que el resto de algoritmos. En concreto, alguno de los basados en ANS obtiene el mejor resultado en $f23$, $f24$ y $f28$, y está a la altura en el resto de funciones compuestas y en la mayoría de multimodales.

Al comparar los resultados de ANS con el resto de participantes de la competición CEC2014, se observa que supera a algunos de los demás algoritmos, pero en general el rendimiento es inferior a la mayoría. Esto en parte se debe a la falta de un ajuste óptimo de los parámetros para cada problema, y al pobre rendimiento de ANS en las funciones unimodales e híbridas.

El comportamiento de los algoritmos basados en ANS en cuanto a la diversidad sigue siendo relativamente errático, y depende mucho del problema en el que se trabaje. Sin embargo, la variación del tamaño de la población en L-ANS y L-HANS aparentemente ayuda a mantener un nivel de diversidad alto antes de converger. Esto se puede observar al comparar las figuras \ref{div-ans} y \ref{div-lhans}, que se han tomado como ejemplo ilustrativo de la situación[^diversity].

[^diversity]: Se pueden encontrar el resto de gráficos de diversidad en \url{https://github.com/fdavidcl/across-neighborhood-search/releases/tag/0.1}.

\begin{figure}[hbtp]
\centering
\includegraphics[width=0.6\textwidth]{stats/out/ans_basic_f3d10.png}
\caption{Evolución de la diversidad en ANS sobre $f3$ en dimensión 10\label{div-ans}}
\end{figure}
\begin{figure}[hbtp]
\centering
\includegraphics[width=0.6\textwidth]{stats/out/l_hans_cmaes_f3d10.png}
\caption{Evolución de la diversidad en L-HANS(CMAES) sobre $f3$ en dimensión 10\label{div-lhans}}
\end{figure}


# Comentarios finales

En este trabajo se ha estudiado la técnica de búsqueda denominada ANS, que presenta características sencillas pero novedosas y un rendimiento prometedor en ciertos tipos de problemas de optimización continua.

Las variantes desarrolladas aportan nuevas ideas y posibles mejoras, pero aún se encuentran en un estado preliminar y requieren de cierto refinamiento para que las mejoras se manifiesten en los resultados. Por un lado, ANS-Memory introduce una alteración en el comportamiento de los individuos; por otro, Hybrid-ANS hibrida la búsqueda con alguna técnica de búsqueda local para optimizar buenas soluciones, y por último L-ANS y L-HANS introducen un cambio dinámico del tamaño de la población para ayudar a que se mantenga cierto nivel de diversidad.

La experimentación realizada revela que estas variantes aún no superan claramente a la versión original de ANS, pero queda abierto el ajuste de los diversos parámetros y el refinamiento de algunas de las ideas, que podrían resultar en verdaderas mejoras del algoritmo. Si así fuera, ANS estaría en condiciones de avanzar hasta ser un algoritmo competitivo con el estado del arte.

# Referencias
