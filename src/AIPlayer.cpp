# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    

    /*
    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch(id){
        case 0:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
            break;
        case 1:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion1);
            break;
        case 2:
            valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, MiValoracion2);
            break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    */

    // ----------------------------------------------------------------- //
    /** TUTORIAL **/
    switch(id) {
        case 0:
            thinkAleatorio(c_piece, id_piece, dice);
            break;
        case 1:
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);
            break;
        case 2:
            thinkFichaMasAdelantada(c_piece, id_piece, dice);
            break;
        case 3:
            thinkMejorOpcion(c_piece, id_piece, dice);
            break;
        case 4:
            thinkMejorOpcionUsandoEspeciales(c_piece, id_piece, dice);
            break;
    }
}

/* --------------------------------------------------|
 _____ _   _ _____ ___  ____  ___    _    _          |
|_   _| | | |_   _/ _ \|  _ \|_ _|  / \  | |         |
  | | | | | | | || | | | |_) || |  / _ \ | |         |
  | | | |_| | | || |_| |  _ < | | / ___ \| |___      |
  |_|  \___/  |_| \___/|_| \_\___/_/   \_\_____|     |
// --------------------------------------------------|
*/

double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador+1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

void AIPlayer::thinkAleatorio(color & c_piece, int & id_piece, int & dice) const{
    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente
    //como lo que se muestran al final de la función.

    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores,
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El id de mi jugador actual.
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableNormalDices(player);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]); // get<i>(tuple<...>) me devuelve el i-ésimo
        c_piece = get<0>(current_pieces[random_id]);  // elemento de la tupla
    }
    else
    {
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor(); // Le tengo que indicar mi color actual al pasar turno.
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color & c_piece, int & id_piece, int & dice) const {
    int player = actual->getCurrentPlayerId();
    vector<int> current_dices;
    vector<tuple<color, int>> current_pieces;

    current_dices = actual->getAllAvailableDices(player);
    vector<int> current_useful_dices;

    for (int i = 0; i < current_dices.size(); i++)
    {
        current_pieces = actual->getAvailablePieces(player, current_dices[i]);
        if (current_pieces.size() > 0)
        {
            current_useful_dices.push_back(current_dices[i]);
        }
    }

    if (current_useful_dices.size() == 0) {
        dice = current_dices[rand() % current_dices.size()];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    } else {
        dice = current_useful_dices[rand() % current_useful_dices.size()];
        current_pieces = actual->getAvailablePieces(player, dice);
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
}

void AIPlayer::thinkFichaMasAdelantada(color & c_piece, int & id_piece, int & dice) const{
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);

    int player = actual->getCurrentPlayerId();
    vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);
    int id_furthest_piece = -1;
    color color_furthest_piece = none;
    int min_distante_goal = 99999;

    for (int i = 0; i < current_pieces.size(); i++) {
        color c = get<0>(current_pieces[i]);
        int id = get<1>(current_pieces[i]);
        int distante_goal = actual->distanceToGoal(c, id);
        if (distante_goal < min_distante_goal) {
            min_distante_goal = distante_goal;
            id_furthest_piece = id;
            color_furthest_piece = c;
        }
    }

    if (id_furthest_piece == -1) {
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    } else {
        id_piece = id_furthest_piece;
        c_piece = color_furthest_piece;
    }
}

void AIPlayer::thinkMejorOpcion(color & c_piece, int & id_piece, int & dice) const{
    /*
    Este agente no aprovecha demasiado las fichas especiales. La clase Parchis 
    proporciona métodos para detectar si en algún movimiento especial se han 
    eliminado fichas, para ver si se han adquirido objetos  especiales  en  el  
    movimiento,  y  muchas  más,  que  pueden  ser  de  interés  para  programar  
    comportamientos más avanzados
    */
    bool better_action = false;
    ParchisBros children = actual->getChildren();

    for (auto it = children.begin(); it != children.end(); ++it) {
        Parchis next_child = *it;
        if (next_child.isEatingMove() or 
            next_child.isGoalMove() or 
            (next_child.gameOver() and next_child.getWinner() == this->jugador)) {
            better_action = true;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
            break;
        }
    }

    if (!better_action) {
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }
}

void AIPlayer::thinkMejorOpcionUsandoEspeciales(color & c_piece, int & id_piece, int & dice) const{
    bool better_action = false;
    ParchisBros children = actual->getChildren();

    for (auto it = children.begin(); it != children.end(); ++it) {
        Parchis next_child = *it;
        if (next_child.isEatingMove() or 
            next_child.isGoalMove() or 
            (next_child.gameOver() and next_child.getWinner() == this->jugador)) {
            better_action = true;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
            break;
        }
    }

    if (!better_action) {
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }

    if (actual->getAvailableSpecialDices(this->jugador).size() > 0) {
        dice = actual->getAvailableSpecialDices(this->jugador)[0];
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    }
}

/**---------------------------------------------------------------|
 __  __ ___ ____    __  __ _____ _____ ___  ____   ___  ____      |
|  \/  |_ _/ ___|  |  \/  | ____|_   _/ _ \|  _ \ / _ \/ ___|     |
| |\/| || |\___ \  | |\/| |  _|   | || | | | | | | | | \___ \     |
| |  | || | ___) | | |  | | |___  | || |_| | |_| | |_| |___) |    |
|_|  |_|___|____/  |_|  |_|_____| |_| \___/|____/ \___/|____/     |
// ---------------------------------------------------------------|
**/
double AIPlayer::ContarDistancia(const Parchis &state, color c) const{
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

    // double score = 0;
    // const int max_distance = (65 +8 + 1);
    // for (int i=0; i<3; i++) {
    //     double distance_from_start = max_distance - state.distanceToGoal(c,i);
    //     score += pow(distance_from_start, 2.);
    // }
}

void AIPlayer::thinkGreedy(color &c_piece, int &id_piece, int &dice) const{
    ParchisBros children = actual->getChildren();
    double best_score = menosinf;

    for (auto it = children.begin(); it != children.end(); ++it) {
        Parchis next_child = *it;
        double score = Heuristica1(next_child, this->jugador);
        if (score > best_score) {
            best_score = score;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
        }
    }
}

double AIPlayer::Heuristica1(const Parchis &state, int player) const{
    double score = 0;

    for (int i = 0; i < 2; i++) {
        double sign = ((i == player) ? 1 : -1);
        // for c in colores
        vector<color> colors = state.getPlayerColors(i);
        for (color c : colors) {
            score += sign * ContarDistancia(state, c);
        }
    }
    return score;
}

double AIPlayer::Heuristica2(const Parchis &state, color c, int player) const{
    // Siguiendo un poco la jerarquia explicada en la memoria:
    double color_score = 0;
    // 65 de moverte hasta la entrada del pasillo
    // final, + 8 casillas del pasillo + 1
    const int max_distance = (65 +8 + 1);
    
    // Puntuaciones por cada acción
    const double score_goal = 10000; // Llegar a la meta
    const double score_corridor = 500; // Llegar al pasillo de la meta
    const double score_eat = 200; // Comer una ficha enemiga / ser comido
    const double score_barrier = 150; // Formar una barrera
    const double score_safe = 100; // Llegar a una casilla segura
    const double score_out = 50; // Sacar una ficha de casa
    const double score_object = 25; // Coger un objeto del tablero

    for (int j = 0; j < 2; j++) {
        double sign = ((j == player) ? 1 : -1);
        vector<color> colors = state.getPlayerColors(j);
        for (color c : colors) {
            double progress = (double) (max_distance - state.distanceToGoal(c,j));
            bool is_safe = state.isSafePiece(c,j);
            bool is_corridor = state.getBoard().getPiece(c,j).get_box().type == final_queue;
            bool is_goal = state.getBoard().getPiece(c,j).get_box().type == goal;
            bool is_home = state.getBoard().getPiece(c,j).get_box().type == home;

            double multplier = max(1.,max(1.5*is_safe, 2.*(is_corridor or is_goal)));
            color_score += pow(progress*multplier, 2.);
            
            // Comprobar si la ficha está en la meta
            if (is_goal) {
                color_score += score_goal;
            }
            
            // Comprobar si la ficha está en el pasillo
            if (is_corridor) {
                color_score += score_corridor;
            }
            
            // Comprobar si la ficha ha comido a una enemiga
            if (state.isEatingMove()) {
                color_score += score_eat;
            }
            
            // Comprobar si la ficha ha formado una barrera
            if (state.isWall(state.getBoard().getPiece(c,j).get_box()) == c or 
                state.isMegaWall(state.getBoard().getPiece(c,j).get_box()) == c) {
                color_score += score_barrier;
            }
            
            // Comprobar si la ficha está en una casilla segura
            if (is_safe) {
                color_score += score_safe;
            }
            
            // Comprobar si la ficha ha salido de casa
            if (is_home) {
                color_score += score_out;
            }
            
            // Comprobar si la ficha ha cogido un objeto
            if (state.itemAcquired()) {
                color_score += score_object;
            }
            color_score *= sign;
        }
    }
    return color_score;
}

// Tercer Encuentro
bool AIPlayer::isVulnerable(const Parchis &estado, color c, int player) const {
    // // Revisa todas las fichas enemigas
    // for (each enemy piece) {
    //     // Si la ficha enemiga puede llegar a la ficha actual en el próximo turno
    //     if (distance between enemy piece and this piece <= 6) { // 6 es el máximo valor de un dado
    //         // Si la ficha no está en una casilla segura
    //         if (!state.isSafePiece(c, player)) {
    //             return true;
    //         }
    //     }
    // }
    // return false;
}

/**
 * @brief Calcula la distancia entre la ficha actual y la ficha enemiga más cercana
 * 
 * @param estado estado actual del juego
 * @param c 
 * @param player jugador actual no enemigo
 * @return double 
 */
double AIPlayer::enemyDistance(const Parchis &estado, color c, int player) const {
    double min_distance = masinf;
    // Revisa todas las fichas del jugador enemigo
    int enemy = (player + 1) % 2;
    estado.getPlayerColors(enemy);
    
    for (auto color : estado.getPlayerColors(enemy)) { // Itero sobre los colores del jugador
        for (auto piece : estado.getBoard().getPieces(color)) { // Itero sobre las fichas de cada color del enemigo
            
            if (piece.get_box().type == (home or normal) and
                estado.getBoard().getPiece(c,).get_box().type == (home or normal)) {  // Si la ficha está en casa o en una casilla normal
                double distance = std::abs(estado.getBoard().getPiece(c,).get_box().num - piece.get_box().num);
                
                if (distance < min_distance) {
                    min_distance = distance;
                }
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

// TODO: Implementar
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

double AIPlayer::Heuristica3(const Parchis &estado, color c, int player) const{

}

/*
    _    _     ____   __  __ ___ _   _    ____  __    _    __  __
   / \  | |   / ___| |  \/  |_ _| \ | |  / /  \/  |  / \   \ \/ /
  / _ \ | |  | |  _  | |\/| || ||  \| | / /| |\/| | / _ \   \  / 
 / ___ \| |__| |_| | | |  | || || |\  |/ / | |  | |/ ___ \  /  \ 
/_/   \_\_____\____| |_|  |_|___|_| \_/_/  |_|  |_/_/   \_\/_/\_\
  
*/
// Nivel MAX e1: actual.children()
// Nivel MIN e2: e1.children() --> no todos los hijos de e1 van a ser MIN --> Sacar un 6 sigue siendo MAX

/*
ParchisBros children = actual->getChildren();
    double best_score = menosinf;

    for (auto it = children.begin(); it != children.end(); ++it) {
        Parchis next_child = *it;
        ParchisBros grandchildren = next_child.getChildren();
        for (auto it2 = grandchildren.begin(); it2 != grandchildren.end(); ++it2) {
            De forma recursiva puedo ir devolviendo la mejor/peor solucion de los hijos dependiendo de si estoy en nodo max/min
        }
        double score = Heuristica1(next_child, this->jugador);
        if (score > best_score) {
            best_score = score;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
        }
    }
*/

double AIPlayer::minimax(Parchis &state, int depth, int player, color &best_piece, int &best_dice, bool maximizingPlayer) const {
    if (depth == 0 || state.gameOver())
        return Heuristica2(state, best_piece, player); // Dependiendo de la Heuristica que escojamos

    if (maximizingPlayer) {
        double maxEval = -std::numeric_limits<double>::infinity();
        ParchisBros children = state.getChildren();
        color tmp_piece;
        int tmp_dice;
        for (auto child = children.begin(); child != children.end(); ++child) {
            bool nextMaximizing = child.getMovedDiceValue() == 6 ? true : false;
            double eval = minimax(*child, depth - 1, player, tmp_piece, tmp_dice, nextMaximizing);
            if (eval > maxEval) {
                maxEval = eval;
                best_piece = child.getMovedColor();
                best_dice = child.getMovedDiceValue();
            }
        }
        return maxEval;
    } else {
        double minEval = std::numeric_limits<double>::infinity();
        ParchisBros children = state.getChildren();
        for (auto child = children.begin(); child != children.end(); ++child) {
            bool nextMaximizing = child.getMovedDiceValue() == 6 ? false : true;
            double eval = minimax(*child, depth - 1, player, best_piece, best_dice, nextMaximizing);
            if (eval < minEval) {
                minEval = eval;
                best_piece = child.getMovedColor();
                best_dice = child.getMovedDiceValue();
            }
        }
        return minEval;
    }
}

/*
    _    _     ____  _   _    _      ____  _____ _____  _    
   / \  | |   |  _ \| | | |  / \    | __ )| ____|_   _|/ \   
  / _ \ | |   | |_) | |_| | / _ \   |  _ \|  _|   | | / _ \  
 / ___ \| |___|  __/|  _  |/ ___ \  | |_) | |___  | |/ ___ \ 
/_/   \_\_____|_|   |_| |_/_/   \_\ |____/|_____| |_/_/   \_\

*/
double AIPlayer::podaAlphaBeta(Parchis &state, int depth, int player, color &best_piece, int &best_dice, bool maximizingPlayer, double alpha, double beta) const {
    if (depth == 0 || state.gameOver())
        return Heuristica2(state, best_piece, player);

    if (maximizingPlayer) {
        double maxEval = -std::numeric_limits<double>::infinity();
        ParchisBros children = state.getChildren();
        color tmp_piece;
        int tmp_dice;
        for (auto child = children.begin(); child != children.end(); ++child) {
            bool nextMaximizing = child.getMovedDiceValue() == 6 ? true : false;
            double eval = podaAlphaBeta(*child, depth - 1, player, tmp_piece, tmp_dice, nextMaximizing, alpha, beta);
            if (eval > maxEval) {
                maxEval = eval;
                best_piece = child.getMovedColor();
                best_dice = child.getMovedDiceValue();
            }
            alpha = std::max(alpha, eval);
            if (beta <= alpha) // Poda Beta
                break;
        }
        return maxEval;
    } else {
        double minEval = std::numeric_limits<double>::infinity();
        ParchisBros children = state.getChildren();
        for (auto child = children.begin(); child != children.end(); ++child) {
            bool nextMaximizing = child.getMovedDiceValue() == 6 ? false : true;
            double eval = podaAlphaBeta(*child, depth - 1, player, best_piece, best_dice, nextMaximizing, alpha, beta);
            if (eval < minEval) {
                minEval = eval;
                best_piece = child.getMovedColor();
                best_dice = child.getMovedDiceValue();
            }
            beta = std::min(beta, eval);
            if (beta <= alpha) // Poda Alpha
                break;
        }
        return minEval;
    }
}
