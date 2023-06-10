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
    
### Codigo completo
```java
double AIPlayer::minimax(Parchis &state, int depth, int player, color &best_piece, int &best_piece_id, int &best_dice, bool maximizingPlayer,
                         double(*heuristica)(const Parchis&,int)) const {
    if(state.gameOver()){
        if(state.getWinner() == player) return gana;
        else return pierde;
    }
    if (depth == 0)
        return heuristica(state, player);

    ParchisBros children = state.getChildren();
    if (maximizingPlayer) {
        double maxEval = menosinf;
        color tmp_piece = none;
        int tmp_dice = -1, tmp_piece_id = -1;
        for (auto child = children.begin(); child != children.end(); ++child) {
            double eval = minimax(*child, depth - 1, player, tmp_piece, tmp_piece_id, tmp_dice, false, heuristica);
            if (eval > maxEval) {
                maxEval = eval;
                best_piece = child.getMovedColor();
                best_dice = child.getMovedDiceValue();
                best_piece_id = child.getMovedPieceId();
            }
        }
        return maxEval;
    } else {
        double minEval = masinf;
        for (auto child = children.begin(); child != children.end(); ++child) {
            double eval = minimax(*child, depth - 1, player, best_piece, best_piece_id, best_dice, true, heuristica);
            if (eval < minEval) {
                minEval = eval;
            }
        }
        return minEval;
    }
}
```
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

# Encuentro Cuarto y Final
## Observaciones
Con las implementaciones anteriores, he llegado al verdadero fracaso, es por ello, que voy a implementar una heuristica desde 0 para al menos intentar ganarle a los dos primeros ninjas, ya que me estoy quedando sin tiempo y tengo bastantes examenes por delante.

Al ver la heurística jugar varias partidas, me estoy dando cuenta de algo **fundamental**, y es que, si tengo una ficha de color azul a 20 de **distancia** para ganar, pero tengo 3 fichas de color verde a 7 de distancia para ganar, la heurística se lo toma de la misma manera que si hay una ficha de color azul a 20 casillas de ganar la partida y 3 fichas de color verde en casa, y no estoy muy seguro de por qué, no obstante, es algo que tengo que modificar para poder ganarle al ninja 3. 

La heurística anterior (heuristica 3) le asigna valores subjetivos a los objetos, algo que no está del todo mal, pero voy a intentar configurarlo de manera que se puedan 'equilibrar' las ponderaciones de los objetos, me refiero a que por ejemplo, si el rival tiene un caparazón azul, mi 'counterplay' sería tener la bocina, para evitar el ataque, además de evitar que el rival avance tantas casillas como se le asigne por comerme a una o dos fichas (dependiendo de si mis fichas están formando una barrera, pueden ser golpeados ambos, algo MUY malo para mi).

No obstante, por las partidas que estoy jugando sin tener en cuenta los objetos, tampoco va tan mal, sino que creo que el jugo de la practica está en lo que he comentado anteriormente sobre las **distancias**. Tambien, he de tener en cuenta y admitir que he buscado informacion acerca de estos juegos sobre distancias, y aunque hayan objetos, caminos distintos, variedades, etc lo imprescindible para analizar la situacion del tablero de la partida, o del estado del juego es lo cerca que estás de ganar, al fin y al cabo, **si mi rival es un deportista de atleta y yo un simple campesino, si a mi me ponen a 1cm de la meta y a él a 1metro, voy a ganar debido a las distancias, independientemente si mi rival me podría superar en las mismas condiciones de distancia**, entonces creo que la clave del éxito está ahí. Además mi teoría se refuerza con el hecho de que los ****objetos** solamente pueden ser escogidos y usados una unica vez, por tanto, **son** una **herramienta** para ganar, pero realmente no hay que hacerle mucho incapié.

Por tanto, creo que la clave está en ir asignando valores subjetivos para ponderar los datos e ir probando las partidas para ver cual es el que tiene mas exito, entonces la idea es la **clave** pero lo que va a hacer ganar la mayoría de veces son las **constantes** que voy modificando para adaptarlos al mejor juego posible.

## Heuristica
Con lo descrito anteriormente, he hecho varias modificaciones que voy a resaltar a continuacion:
* Enfocar mas el problema a las distancias que a los objetos, pienso que los objetos son una **herramienta** para ganar pero no condicion necesaria como viene a ser las distancias a la meta
* Sinopsis entre objetos, por ejemplo:
    * El **rival** posee **caparazon** azul o rojo, por tanto un buen tablero debe de contener a la **bocina** para bloquear el ataque e impedir que el enemigo avance junto con que elimine la/s ficha/s con la que interactue el caparazon
    * Solamente he considerado los objetos que proporcionan distancias, es decir, la bala, la seta, los caparazones (porque eliminan fichas enemigas), y les he asignado valores cerca de lo que proporcionan en distancia, es decir, como la bala suma 45 casillas, pues le he asignado un valor de 40, para la seta igual, suma minimo 8, por tanto le asigno un valor de 6. Esto ha incrementado mucho la mejora del bot.

* Añadir la condicion de si hay fichas en casa, es peor, algo sencillo de entender pero que no habia tenido en cuenta anteriormente.
* Siempre intentar valorar que mis fichas esten en el tablero, esto es algo que va enlazado con las distancias.

Yo creo que la clave ha sido lo siguiente:
* Al considerar las distancias, el mejor caso es cuando yo tengo una distancia pequeña y mi enemigo una distancia grande, pero al realizar la Poda Alfa Beta, se van a escoger tableros donde el valor de la heurística sea lo más grande posible, esto se traduce en que tanto mi rival como yo tengamos distancias grandes, algo que evidentemente no se quiere, por tanto, despues de echar un buen rato pensando, se me ha ocurrido hacer:
    * Para cada jugador
        * Para cada color, recorrer todas las fichas del mismo y sumar la distancia de cada ficha hacia el objetivo, como queremos que nuestras fichas esten lo mas cerca posible de la meta, lo guardo en una variable que vaya sumando el conjunto de distancias de las fichas de cada color.
        * Compruebo si las fichas están en casillas seguras, si estan en el tablero, etc.. Esto lo he tomado de la heuristica ValoracionTest

Con todo esto, devuelvo la distancia de mis fichas en negativo, teniendo en cuenta la distancia minima (que es lo mismo que maxDistancia - miDistancia) y le sumo algo de las distancias maximas (max*0.35 para que se tenga algo en cuenta, la constante la he calculado a partir de desarrollo empirico), para que tambien se tengan en cuenta. Devuelvo mi puntuacion respecto a objetos, fichas en casillas seguras, etc. Va bastante bien, de hecho le vence a los 3 ninjas, aunque se podria mejorar un poco, estoy satisfecho.

El codigo final seria el siguiente:
```java
double AIPlayer::Heuristica4(const Parchis &state, int player) {
    int enemy_player = (player + 1) % 2;

    if (state.getWinner() == player)    return gana;
    else if (state.getWinner() == enemy_player) return pierde;

    // Distancias y valores para los objetos
    vector<int> distances;
    int player_score = 0, enemy_score = 0;

    for (auto color : state.getPlayerColors(player)) {
        int sum = 0;
        int piece_id = 0;
        for (auto piece : state.getBoard().getPieces(color)) {
            int distance = state.distanceToGoal(color, piece_id);
            sum += distance;

            if (state.isSafePiece(color, piece_id)) player_score++;
            else if (piece.get_box().type == goal)  player_score += 5;
            else if (piece.get_box().type == home)  player_score -= 10;

            piece_id++;
        }
        distances.push_back(sum);
    }

    int min_player = min(distances[0], distances[1]);
    int max_player = max(distances[0], distances[1]);
    int sum_player = min_player + 0.35 * max_player; // Para tener algo en cuenta las fichas del otro color
    distances.clear();

    for (auto enemyColor : state.getPlayerColors(enemy_player)) {
        int sum = 0;
        int piece_id = 0;
        for (auto piece : state.getBoard().getPieces(enemyColor)) {
            int distance =  state.distanceToGoal(enemyColor, piece_id);
            sum += distance;

            if (state.isSafePiece(enemyColor, piece_id)) enemy_score++;
            else if (piece.get_box().type == goal)  enemy_score += 5;
            else if (piece.get_box().type == home)  enemy_score -= 10;

            piece_id++;
        }
        distances.push_back(sum);
    }

    int min_enemy = min(distances[0], distances[1]);
    int max_enemy = max(distances[0], distances[1]);
    int sum_enemy = min_enemy + 0.35 * max_enemy; // Para tener algo en cuenta las fichas del otro color

    for (auto specialDices : state.getAvailableSpecialDices(player)) {
        for (auto enemySpecialDices : state.getAvailableSpecialDices(enemy_player)) {
            if (specialDices == horn and enemySpecialDices == (red_shell or blue_shell))    player_score += 30;
            else if (specialDices == (red_shell or blue_shell) and enemySpecialDices == horn)   enemy_score += 30;

            if (specialDices == bullet) player_score += 40;
            if (specialDices == mushroom)   player_score += 6;
            if (specialDices == red_shell)  player_score += 10;
            if (specialDices == blue_shell) player_score += 15;

            if (enemySpecialDices == bullet)    enemy_score += 40;
            if (enemySpecialDices == mushroom)  enemy_score += 6;
            if (enemySpecialDices == red_shell)  enemy_score += 10;
            if (enemySpecialDices == blue_shell) enemy_score += 15;
        }
    }

    return -sum_player + sum_enemy + player_score - enemy_score;
}
```

## Implementación Poda Alfa-Beta
He mejorado levemente el codigo, porque como se puede apreciar en el pseudocodigo implementado con anterioridad, hay algunos posibles problemas que he podido percatar:
* La referencia a best_piece y best_dice en los nodos no máximos: Se están asignando los valores de best_piece y best_dice en cada nodo, incluso en aquellos nodos que no son máximos (es decir, aquellos nodos que están minimizando). En el algoritmo de minimax con poda alfa-beta, solo debería registrar los movimientos en los nodos de maximización, porque esos son los movimientos que el jugador de IA realmente hará.
* La referencia a tmp_piece y tmp_dice: Tengo una referencia a las variables tmp_piece y tmp_dice que no parecen estar utilizadas correctamente. Las he pasado a la función recursiva pero no estoy usando los valores resultantes.
* El uso de la variable nextMaximizing: Estoy determinando si el siguiente jugador es maximizador en función del valor del dado. El problema que no habia tenido en cuenta, era que el dado puede ser un 6, o un 10 o 20, etc. Por tanto, deberia fijarme si en el nivel en el que estoy, es del jugador oponente o el mio.

Ahora, la funcion es correcta y da buenos resultados, he aqui el codigo:
```java
double AIPlayer::podaAlphaBeta(const Parchis *state, const int depth, const int player, color &best_piece, int &best_piece_id, 
                               int &best_dice, double alpha, double beta,
                               double(*heuristica)(const Parchis&, int)) const {
    
    if (depth == PROFUNDIDAD_ALFABETA or state->gameOver())
        return heuristica(*state, player);

    ParchisBros children = state->getChildren();
    ParchisBros::Iterator it = children.begin();
    bool es_nodo_max = state->getCurrentPlayerId() == player;

    if (es_nodo_max) {
        while (it != children.end()) {
            double eval = podaAlphaBeta(&(*it), depth + 1, player, best_piece, best_piece_id, best_dice, alpha, beta, heuristica);

            if (eval > alpha) {
                alpha = eval;
                if (depth == 0) {
                    best_piece = it.getMovedColor();
                    best_piece_id = it.getMovedPieceId();
                    best_dice = it.getMovedDiceValue();
                }
            }

            if (alpha >= beta)   return beta;
            ++it;
        }
        return alpha;

    } else {
        while (it != children.end()) {
            double eval = podaAlphaBeta(&(*it), depth + 1, player, best_piece, best_piece_id, best_dice, alpha, beta, heuristica);
            beta = min(beta, eval);
            if (alpha >= beta)  return alpha;
            ++it;
        }
        return beta;
    }
}
```