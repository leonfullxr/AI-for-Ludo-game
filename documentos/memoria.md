# Memoria de la Practica 3
## Indice de contenidos

# Introduccion
En esta memoria iré explicando el **avance** de la práctica, es decir, los distintos **planetamientos** de la práctica junto con sus distintas **implementaciones** y métodos.
A lo largo de la memoria se va a tener en cuenta lo siguiente:
* **Diseño** de los métodos propuestos
* **Implementación** de los métodos propuestos
* **Alternativas** de los métodos propuestos
* **Ventajas** y **Desventajas** de los métodos propuestos
    * Alternativas de los métodos propuestos
        * El por qué falla/triunfa un método/algoritmo

Aunque en resumen, lo realmente importante es dar con una **buena funcion heurística**, algo que a simple vista no se puede razonar. Es por ello, que empezaré por lo más básico (el tutorial) hasta avanzar a lo que sería la implementación final.

# Objetivo
El objetivo de esta memoria es intentar ir explicando y plasmando las ideas / correcciones / problemas que van surgiendo a lo largo de la práctica con el objetivo de plasmar cierto avance, además de poder disfrutar de la puntuación máxima asignada a esta tarea.

Además, lo realmente crucial de esta práctica es la **heurística**, y lo que seguramente vaya a tardar más en implementar/desarrollar, pues la implementación del algoritmo MIN/MAX además de la poda ALPHA/BETA es simplemente saber recorrer los hijos y aplicar la heurística.

# Primer Encuentro
Lo primero de todo es haber empezado y completado el tutorial, algo sencillo de hacer, pues es nada más que copiar/pegar y entender el código proporcionado por los profesores.

Al probar los distintos métodos proporcionados por el tutorial, podemos observar un gran cambio de haber tenido un movimiento aleatorio total, a ir teniendo en cuenta la distancia final necesaria para la meta, junto con los distintos aspectos de lo cerca que está la ficha de llegar a la meta o a una zona segura, intentar comer cualquier ficha que pueda, la formación de una barrera, etc.

No obstante, lo que no se plantea son las utilidades de las fichas y los objetos que hay por el tablero, que pueden ayudar significativamente a ganar la partida, y algo que tendremos en cuenta a lo largo de la práctica.

## Aspectos a tener en cuenta para la Heurística
Del este primer avance podemos concluir que tendremos que tener en cuenta los distintos aspectos del juego para añadir en nuestra heurística:
* Formar una **barrera**
* La **distancia** requerida para llegar a la **meta**
* La **distancia** requerida para llegar a una **casilla segura**
* **Comer** cualquier ficha posible
* Intentar **priorizar** cuando se pueda, **sacar** las fichas de **casa**, es decir, mientras más fichas tengamos en el tablero, más chances tenemos de coger objetos, comer fichas enemigas/aliadas, crear barreras, etc. Tener fichas en el tablero aumenta significativamente nuestros chances de ganar
Implementación básica ajustada a los puntos resumidos anteriormente:
```c++
double funcionHeuristica() {
    double color_score = 0;
    // 65 de moverte hasta la entrada del pasillo
    // final, + 8 casillas del pasillo + 1
    const int max_distance = (65 +8 + 1);

    for (int j = 0; j < num_pieces; j++) {
        double progress = (double) (max_distance - state.distanceToGoal(c,j));
        bool is_safe = state.isSafePiece(c,j);
        bool is_corridor = state.getBoard().getPiece(c,j).get_box().type == final_queue;
        bool is_goal = state.getBoard().getPiece(c,j).get_box().type == goal;

        double multplier = max(1.,max(1.5*is_safe, 2.*(is_corridor or is_goal)));
        color_score += pow(progress*multplier, 2.);
    }
    return color_score;
}
```
Esta implementacion es un ejemplo de cómo podemos tener en cuenta los distintos estados de una ficha y sus posibles movimientos próximos, algo muy simple por el que podemos tomar como base para ir refinando la heurística.

## Estilos de juego
De lo anterior, y de cualquier juego competitivo, podemos concluir que podemos, de cierta manera, **orientar** a nuestra IA para que tome distintas formas de juego, y me refiero a lo siguiente:
* Modo de juego **agresivo**, donde la IA va a intentar maximizar comer/eliminar las fichas del oponente
* Modo de juego **pasivo/defensivo**, donde la IA va a intentar maximizar las barreras, las casillas seguras, etc.
* Modod de juego **híbrido**, donde nuestra IA tiene que ser lo suficientemente capacitado para inclinarse hacia un modo de juego particular dada una situación en el tablero, algo que realmente queremos para la práctica y lo que intentaré implementar. No obstante, no creo que sin redes neuronales pueda hacerlo, ya que, de cierta manera tengo que priorizar un estilo o movimiento sobre otros.

## Resumen
Entonces, al igual que en los juegos competitivos de la vida real, dependiendo del estilo de juego del oponente, podemos adaptarnos para sacar el máximo beneficio, no obstante, esto es algo que no se va a poder hacer en esta práctica, pero es algo de lo que intentaré focalizar dependiendo del éxito de las pruebas realizadas para cada estilo de juego.

# Segundo Encuentro
Como era de esperar, lo que tenemos hecho hasta ahora es demasiado básico como para considerarlo una buena heurística, por el simple hecho de que no tenemos en cuenta los objetos de poder del tablero, además del hecho de que tenemos que **priorizar** de alguna manera los **distintos estados** en los que se encuentra una ficha.

Por tanto, al igual que en la vida real, debemos asignar valores a las distintas posiciones que puede tomar una ficha, refiriendome a lo que describimos en el primer encuentro, pero habría que añadir los distintos estados que podemos adquirir de los distintos objetos que podremos ir adquiriendo por el tablero.

A continuación, voy a definir los **poderes de estado** que puede tener una ficha, de los cuales considero importante:
* **Invencible** --> Proviene de la **Estrella**
* **Fantasma** --> Proviene del **Boo**
* **Megaficha** --> Proviene del **MegaChampiñon**

Ahora, hay poderes que no nos sirven dependiendo del estado en el que estemos, es decir, un ejemplo para mostrarlo:
> Casilla segura !=! fantasma

No tiene sentido aplicar un estado fantasma a una ficha que esté posicionada en una casilla segura, por razones obvias.
> Invencible !=! Cerca de Casa

Realmente no tiene mucho valor asignar un estado invencible a una ficha que esté cerca de su propia casa, pues es preferible que se lo asigne a una ficha que esté cerca de la meta o incluso hacer que la ficha avance desde un primer momento.

Entonces, los poderes son convenientes dependiendo de los poderes que tenga el rival, pues apenas nos sirve de mucho tener una bocina si el rival no posee el caparazón.

## Heuristica
### Planteamiento
Resumiendo un poco lo anterior, los *poderes de estado* van a tener **valores no predeterminados**, es decir, que dependiendo de los objetos del rival, les voy a asignar un valor mayor o menor, dependiendo de si el rival contiene objetos que puedan eliminar mis fichas del tablero, pues aparte de comerse mis fichas, se le otorgan posiciones de avance en las casillas, un auténtico beneficio.

Entonces, voy a intentar pulir la Heurística básica que tengo implementado del tutorial, para a continuación introducir los objetos del tablero y conseguir tener una mejor heurística.

### Jerarquia
Al implementar la heuristica, voy a priorizar lo siguiente de mayor a menor:
* Llegar a la **meta**: *esto implica **cualquier** moviemiento que permita a la ficha llegar a la meta*. *Ej* --> *comerse a una ficha, usar la bala, etc..*
    * Tambien implica el **sacrificio** de una ficha aliada, aqui es donde se distingue el uso de la **cooperacion** entre colores, es decir, una toma el rol de **apoyo** y el otro el rol de **ataque**. Aunque esto lo implementaré un poco más adelante en la práctica, ya que no es sencillo de ver.

* Llegar al **pasillo** de la meta (las 8 casillas antes de la meta)
* **Comer** una ficha **enemiga**
* Formar una **barrera** *(de alguna manera, formar una barrera implica que ambas fichas estén en una casilla de posicionamiento seguro)*
* Llegar a una **casilla segura**
* **Sacar** una ficha de **casa**
* **Coger** un **objeto** del tablero
* Ya lo último sería simplemente **analizar** la **posicion** en la que esta la ficha, el valor corresponderia con la distancia necesaria para llegar a la meta.

### Inconvenientes
Como era de esperar, es evidente que esta heurística no es buena, pues trata a todos los objetos por igual, es decir, lo suyo es aplicarle valores a cada objeto dependiendo del estado del tablero, algo que tendré en cuenta al mejorar esta heurística.
* Asignarle **valores** **dinámicos** a los **objetos** (dependiendo del estado del tablero los objetos son mejores/neutros).
* Asignarle **prioridades** a los objetos, es decir, si hay una **ficha** enemiga **cerca** de la **meta** es muy bueno usar la **concha azul**, incluso si hay dos fichas enemigas formando una barrera, se usaría la concha para eliminar ambas fichas que forman la barrera y darle via libre a alguna ficha que quedó bloqueada por la barrera.

### Pseudocodigo
```java

```

## Implementacion Min/Max
### Pseudocodigo
```java
function minimax(position, depth, maximizingPlayer)
    if depth == 0 or game over in position
        return static evaluation of position

    if maximizingPlayer
        maxEval = -infinity
        for each child of position
            eval = minimax(child, depth-1, false)
            maxEval = max(maxEval,eval)
        return maxEval
    else 
        minEval = +infinity
        for each child of position
            eval = minimax(child, depth-1, true)
            minEval = min(minEval,eval)
        return minEval
    
// initial call 
minimax(currentPosition, PROFUNDIDAD_MINIMAX, true)
```
Se puede apreciar que cuando la profundidad llega a ser 0, debemos devolver la llamada a la funcion heuristica para analizar el estado del tablero al que llega la profundidad maxima.

El pseudocodigo es muy bonito, pero a la hora de la implementación si que es cierto que queda un poco más feo, no obstante, voy a intentar dejarlo lo mas bonito posible. Aqui lo realmente importante es que funcione correctamente la implementación, pues a partir de ahí, implementar alpha/beta es bastante sencillo.

# Tercer Encuentro
Una vez hecho la implementacion de la funcion minimax junto con la implementacion de la heuristica basica que tiene en cuenta los objetos del tablero, podemos pasar a terminar la implementacion del algoritmo alpha/beta junto con un desarrollo mas avanzado de la heuristica.

Con respecto a la implementacion del algoritmo alpha/beta es bastante sencillo, pues solamente hay que ajustar un par de cosas respecto de la funcion minimax, es decir, que en vez de devolver el valor maximo o minimo, devolvemos el valor de alpha o beta, dependiendo de si es el jugador max o min. Además, en este encuentro, voy a destacar varias cosas sobre la heuristica anterior que creo que va a ser importante para el desarrollo de la heuristica final.

## Heuristica
### Planteamiento
Mejoras para tener una heuristica mas sofisticada:<br>
- 1.Variabilidad de la puntuación en base a la **posición** de los **enemigos**: La heurística podría ser mejorada tomando en cuenta la posición de las fichas enemigas. Por ejemplo, podrías aumentar la puntuación si una de tus fichas está cerca de una ficha enemiga y tienes una alta probabilidad de comerla en los próximos turnos.
- 2.Priorizar mover fichas que están más **cerca** de la **meta**: En lugar de tratar a todas las fichas igualmente, podrías dar una mayor prioridad a mover las fichas que están más cerca de la meta. Esto se puede hacer aumentando su puntuación en la heurística.
- 3.Considerar la probabilidad de ser **comido** por un **enemigo**: De forma similar al primer punto, podrías disminuir la puntuación de una ficha si está en peligro de ser comida por un enemigo en los próximos turnos.
- 4.**Estrategia** de **salida**: Podrías tener una estrategia para decidir cuándo es el mejor momento para sacar una ficha de la casa. Por ejemplo, podría ser beneficioso esperar hasta que todas las fichas enemigas estén lejos de la casa.
- 5.Agregar una **penalización** para las jugadas que dejan a las **fichas vulnerables**: Aunque es beneficioso comer fichas enemigas, deberías considerar las consecuencias de tus acciones. Si mover una ficha a una posición determinada aumenta la posibilidad de ser comido en el próximo turno, es posible que desees descontar algunos puntos de esa acción.
- 6.Devolver el **valor máximo** si el estado del tablero contiene la **victoria** del jugador, algo que no he tenido en cuenta anteriormente, pues no habria que realizar ninguna comprobacion mas, el jugador simplemente ha ganado.
Hay muchas otras formas de mejorar la heurística. Estas son solo algunas ideas para empezar, no obstante, voy a intentar implementarlo con funciones auxiliares para que sea mas sencillo de entender e implementar. Al final, lo que realmente decide si la heuristica es mejor es a traves de la experimentacion, es decir, probando diferentes heurísticas y viendo cual es la que mejor funciona. Esto tendre que hacerlo mediante algun script que me permita jugar contra mi mismo y ver cual es la que mejor funciona. El script lo implementaré en el siguiente encuentro.

### Pseudocodigo
```java
funcion Heuristica3() {
    // todo lo mismo que en la Heuristica2 pero añadiendo lo siguiente
    if (jugador gana)   return +inf;
    if (jugador pierde) return -inf;
    // ...
    const double score_vulnerable = -500; // Ser vulnerable a ser comido
    const double score_enemy_near = 300; // Un enemigo está cerca y puede ser comido

    for (color c : colors) {
        // ...
        double multplier = max(1.,max(1.5*is_safe, 2.*(is_corridor or is_goal)));
        color_score += pow(progress*multplier, 2.);

        // Comprobar si la ficha esta en una posicion vulnerable
        if (isVulnerable(c, j)) {
            color_score += score_vulnerable;
        }

        // Comprobar si hay un enemigo cerca que pueda ser comido en los siguientes turnos
        if (enemyDistance(c, j) <= 6) { // 6 is the max roll of a dice
            color_score += score_enemy_near;
        }

        // Add extra score for pieces that are beneficial to leave home
        if (isBeneficialToLeaveHome(c, j)) {
            color_score += score_out * 1.5; // Extra bonus for beneficial moves
        }

        // ... 
    }

    // ...
}
```
Para añadir las mejoras propuestas anteriormente a la heurística, primero necesitamos definir nuevas funciones y añadir nuevas constantes de puntuación:
- 1.`double enemyDistance(color c, int player)`: Esta función debería devolver la distancia mínima a una ficha enemiga. Podrías usarla para determinar la probabilidad de comer una ficha enemiga o de ser comido por una.
- 2.`bool isVulnerable(color c, int player)`: Esta función debería devolver true si una ficha está en peligro de ser comida por una ficha enemiga en el próximo turno.
- 3.`bool isBeneficialToLeaveHome(color c, int player)`: Esta función debería devolver true si es beneficioso sacar una ficha de la casa. Esto podría depender de la posición de las fichas enemigas.

Voy a tambien proporcionar un pseudocodigo para las funciones axiliares anteriores:
```java
bool AIPlayer::isVulnerable(color c, int player) const {
    // Revisa todas las fichas enemigas
    for (each enemy piece) {
        // Si la ficha enemiga puede llegar a la ficha actual en el próximo turno
        if (distance between enemy piece and this piece <= 6) { // 6 es el máximo valor de un dado
            // Si la ficha no está en una casilla segura
            if (!state.isSafePiece(c, player)) {
                return true;
            }
        }
    }
    return false;
}

double AIPlayer::enemyDistance(color c, int player) const {
    double min_distance = some large number; // E.g., 100
    // Revisa todas las fichas enemigas
    for (each enemy piece) {
        double distance = calculate distance between this piece and enemy piece;
        // Si la distancia es más corta que la menor encontrada hasta ahora
        if (distance < min_distance) {
            min_distance = distance;
        }
    }
    return min_distance;
}

bool AIPlayer::isBeneficialToLeaveHome(color c, int player) const {
    // Implement your own logic here. For example, one basic strategy could be:
    // Si todas las fichas están en casa o en la meta, es beneficioso sacar una de casa
    if (all pieces are in home or goal) {
        return true;
    }
    // Si una ficha en casa podría moverse a una casilla segura en el próximo turno
    if (distance to nearest safe spot from home <= 6) { // 6 es el máximo valor de un dado
        return true;
    }
    return false;
}
```

## Implementación Poda Alfa-Beta
No tiene mucha complicacion con respecto a la funcion minimax, simplemente voy a dejar el pseudocodigo que he utilizado para implementarla:

### Pseudocodigo
```java
function minimax(position, depth, alpha, beta, maximizinPlayer)
	if depth == 0 or game over in position
		return static evaluation of position;

	if maximizingPlayer
		maxEval = -infinity
		for each child of position
			eval = minimax(child, depth, -1, alpha, beta,  false);
			maxEval = max(maxEval,eval);
			alpha = max(alpha,eval);
			if beta <= alpha break;
		return maxEval;
	else
		minEval = +infinity
		for each child in position
			eval = minimax(child, depth, -1, beta, true);
			minEval = min(minEval,eval);
			beta = min(beta,eval);
			if beta <= alpha break;
		return minEval;
```

# Encuentro Cuarto
## Observaciones
Al ver la heurística jugar varias partidas, me estoy dando cuenta de algo **fundamental**, y es que, si tengo una ficha de color azul a 20 de **distancia** para ganar, pero tengo 3 fichas de color verde a 7 de distancia para ganar, la heurística se lo toma de la misma manera que si hay una ficha de color azul a 20 casillas de ganar la partida y 3 fichas de color verde en casa, y no estoy muy seguro de por qué, no obstante, es algo que tengo que modificar para poder ganarle al ninja 3. 

La heurística anterior (heuristica 3) le asigna valores subjetivos a los objetos, algo que no está del todo mal, pero voy a intentar configurarlo de manera que se puedan 'equilibrar' las ponderaciones de los objetos, me refiero a que por ejemplo, si el rival tiene un caparazón azul, mi 'counterplay' sería tener la bocina, para evitar el ataque, además de evitar que el rival avance tantas casillas como se le asigne por comerme a una o dos fichas (dependiendo de si mis fichas están formando una barrera, pueden ser golpeados ambos, algo MUY malo para mi).

No obstante, por las partidas que estoy jugando sin tener en cuenta los objetos, tampoco va tan mal, sino que creo que el jugo de la practica está en lo que he comentado anteriormente sobre las **distancias**. Tambien, he de tener en cuenta y admitir que he buscado informacion acerca de estos juegos sobre distancias, y aunque hayan objetos, caminos distintos, variedades, etc lo imprescindible para analizar la situacion del tablero de la partida, o del estado del juego es lo cerca que estás de ganar, al fin y al cabo, **si mi rival es un deportista de atleta y yo un simple campesino, si a mi me ponen a 1cm de la meta y a él a 1metro, voy a ganar debido a las distancias, independientemente si mi rival me podría superar en las mismas condiciones de distancia**, entonces creo que la clave del éxito está ahí. Además mi teoría se refuerza con el hecho de que los ****objetos** solamente pueden ser escogidos y usados una unica vez, por tanto, **son** una **herramienta** para ganar, pero realmente no hay que hacerle mucho incapié.

Por tanto, creo que la clave está en ir asignando valores subjetivos para ponderar los datos e ir probando las partidas para ver cual es el que tiene mas exito, entonces la idea es la **clave** pero lo que va a hacer ganar la mayoría de veces son las **constantes** que voy modificando para adaptarlos al mejor juego posible.