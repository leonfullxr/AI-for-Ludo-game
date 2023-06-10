#ifndef __AI_PLAYER_H__
#define __AI_PLAYER_H__

# include "Attributes.h"
# include "Player.h"

// Necesito hacer esto porque el compilador no reconoce los objetos de la clase Piece, al no declararse
// la clase Piece.h antes que AIPlayer.h
class Piece;

class AIPlayer: public Player{
    protected:
        //Id identificativo del jugador
        const int id;
    public:
        /**
         * @brief Constructor de un objeto AIPlayer
         * 
         * @param name Nombre del jugador
         */
        inline AIPlayer(const string & name):Player(name), id(1){};
        
        /**
         * @brief Constructor de un objeto AIPlayer 
         * 
         * @param name Nombre  del jugador
         * @param id Id del jugador
         */
        inline AIPlayer(const string & name, const int id):Player(name), id(id){};

        /**
         * @brief Función que percibe el el parchís y al jugador actual.
         * Asigna el tablero en actual y el id del jugador.
         * 
         * @param p Instancia Parchis que se quiere percibir
         */
        inline virtual void perceive(Parchis &p){Player::perceive(p);}

        /**
         * @brief Función abstracta que define el movimiento devuelto por el jugador.
         * Llama a la función movePiece con el valor asignado a los parámetros pasados 
         * por referencia.
         * 
         * @return true
         * @return false 
         */
        virtual bool move();
        
        /**
         * @brief Función que se encarga de decidir el mejor movimiento posible a 
         * partir del estado actual del tablero. Asigna a las variables pasadas por
         * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
         * 
         * @param c_piece Color de la ficha
         * @param id_piece Id de la ficha
         * @param dice Número de dado
         */
        virtual void think(color & c_piece,  int & id_piece, int & dice) const;

        /**
         * @brief Método que determina si el player es inteligente (decide el mejor movimiento)
         * o no. True para AIPlayer.
         * 
         * @return true 
         * @return false 
         */
        inline virtual bool canThink() const{return true;}

        /**
         * @brief Heurística de prueba para validar el algoritmo de búsqueda.
         * 
         * @param estado Instancia de Parchis con el estado actual de la partida.
         * @param jugador Id del jugador actual (0 o 1)
         * @return double 
         */
        static double ValoracionTest(const Parchis &estado, int jugador);

        /**
         * @brief Propuesta de declaración de la función poda alfa-beta.
         * La propuesta es solo sugerencia, los parámetros de la declaración podrían variar.
         */
        //double Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const;
    /**
       _____ _   _ _____ ___  ____  ___    _    _     
      |_   _| | | |_   _/ _ \|  _ \|_ _|  / \  | |    
        | | | | | | | || | | | |_) || |  / _ \ | |    
        | | | |_| | | || |_| |  _ < | | / ___ \| |___ 
        |_|  \___/  |_| \___/|_| \_\___/_/   \_\_____|
        **/  
        void thinkAleatorio(color & c_piece,  int & id_piece, int & dice) const;
        void thinkAleatorioMasInteligente(color & c_piece,  int & id_piece, int & dice) const;
        void thinkFichaMasAdelantada(color & c_piece,  int & id_piece, int & dice) const;
        void thinkMejorOpcion(color & c_piece,  int & id_piece, int & dice) const;
        void thinkMejorOpcionUsandoEspeciales(color & c_piece,  int & id_piece, int & dice) const;

     /** __  __ ___ ____    __  __ _____ _____ ___  ____   ___  ____  
        |  \/  |_ _/ ___|  |  \/  | ____|_   _/ _ \|  _ \ / _ \/ ___| 
        | |\/| || |\___ \  | |\/| |  _|   | || | | | | | | | | \___ \ 
        | |  | || | ___) | | |  | | |___  | || |_| | |_| | |_| |___) |
        |_|  |_|___|____/  |_|  |_|_____| |_| \___/|____/ \___/|____/ 
        **/
        // Primer Encuentro
        void thinkGreedy(color & c_piece,  int & id_piece, int & dice) const;
        double simpleHeuristicSingleColor(const Parchis &estado, color c) const;
        double Heuristic1(const Parchis &estado, int jugador) const;
        double ContarDistancia(const Parchis &estado, color c) const;
        // Segundo Encuentro
        static double Heuristica2(const Parchis &estado, int player);
        double minimax(Parchis &state, int depth, int player, color &best_piece, int &best_piece_id, int &best_dice, bool maximizingPlayer,
                         double(*heuristica)(const Parchis&,int)) const;
        // Tercer Encuentro
        static double Heuristica3(const Parchis &estado, int player);
        /**
         * @brief Checks if a given game piece is vulnerable to an enemy attack.
         * @param estado The current game state.
         * @param piece The game piece to check vulnerability for.
         * @param player The ID of the player who owns the piece.
         * @return true if the game piece is vulnerable, false otherwise.
         *
         * The function checks if the piece can be eaten by an enemy piece, given the current state of the game.
         * The piece is considered vulnerable if it's not a star or boo piece, and there's an enemy piece that can reach it.
         */
        bool isVulnerablePiece(const Parchis &estado, const Piece &piece, int player) const;
        /**
         * @brief Calculates the shortest distance between the current player's pieces and the closest enemy pieces that can eat the current players piece.
         * @param estado The current game state.
         * @param player The ID of the current player.
         * @return The shortest distance to a vulnerable enemy piece.
         *
         * The function iterates through all the enemy pieces, and for each, it calculates the distance to the current player's pieces.
         * The shortest of these distances is returned.
         */
        double enemyDistance(const Parchis &estado, int player) const;
        /**
         * @brief Determines whether it's beneficial for a piece to leave home, based on the current game state.
         * @param estado The current game state.
         * @param piece The game piece to make the decision for.
         * @param player The ID of the player who owns the piece.
         * @return true if it's beneficial for the piece to leave home, false otherwise.
         *
         * The function checks if the piece can move to a safe box on the next turn. It considers both normal and special dices.
         * If the piece can move to a safe box, and there are no obstructions on the path, the function returns true.
         * Otherwise, it returns false.
         */
        bool isBeneficialToLeaveHome(const Parchis &estado, const Piece &piece, int player) const;
        /**
         * Determina si una ficha está justo al salir de casa.
         * @param piece La ficha a comprobar.
         * @return Verdadero si la ficha está justo al salir de casa, falso en caso contrario.
         */
        bool isPieceInHome(const Piece &piece) const;
        /**
         * Determina si una ficha está a una casilla de distancia del corredor.
         * @param piece La ficha a comprobar.
         * @return Verdadero si la ficha está a una casilla de distancia del corredor, falso en caso contrario.
         */
        bool isPieceOneSquareFromCorridor(const Piece &piece) const;
        /**
         * Verifica si hay un camino claro entre dos casillas, teniendo en cuenta la pieza dada.
         * 
         * @param state El estado actual del juego.
         * @param b1 La casilla inicial del camino a comprobar.
         * @param b2 La casilla final del camino a comprobar.
         * @param piece La ficha que se está moviendo.
         * @return Verdadero si el camino entre las dos casillas está despejado, falso en caso contrario.
         */
        bool clearPathBetweenTwoSquares(const Parchis &state, const Box &b1, const Box &b2, const Piece &piece) const; // Comprueba si se puede transitar desde b1 hasta b2
        /**
         * Calcula el tipo de casilla en la que se encontrará una ficha después de un incremento de posición.
         * 
         * @param piece La ficha cuya posición se incrementará.
         * @param positionIncrement El incremento de la posición de la ficha.
         * @return La casilla en la que se encontrará la ficha después del incremento de posición.
         */
        Box calculateBoxType(const Piece &piece, const int positionIncrement) const;

        /**
         * Determina si una ficha puede ser comido por la concha roja de un jugador enemigo.
         * 
         * @param state El estado actual del juego.
         * @param piece La ficha que lanza el objeto
         * @param enemyPlayer El jugador enemigo
         * @param targetPiece La ficha enemiga que se está considerando para ser comido.
         * @return Verdadero si la ficha puede ser comido por la concha roja del jugador que la lanza, falso en caso contrario.
         */
        bool pieceCanBeEatenByRedShell(const Parchis &state, const Piece &piece, int enemyPlayer, const Piece &targetPiece) const;
        
        /**
         * Determina si una ficha puede ser comido por la concha azul de un jugador enemigo.
         * 
         * @param state El estado actual del juego.
         * @param piece La ficha que lanza el objeto
         * @param enemyPlayer El jugador enemigo
         * @param targetPiece La ficha enemiga que se está considerando para ser comido.
         * @return Verdadero si la ficha puede ser comido por la concha azul del jugador que la lanza, falso en caso contrario.
         */
        bool pieceCanBeEatenByBlueShell(const Parchis &state, const Piece &piece, int enemyPlayer, const Piece &targetPiece) const;
        static double evaluateItems(const Parchis& state, const int player);
        double podaAlphaBeta(const Parchis *state, const int depth, const int player, color &best_piece, int &best_piece_id, int &best_dice, double alpha, double beta, double(*heuristica)(const Parchis&,int)) const;
        static double Heuristica4(const Parchis &state, int player);

    private:
        enum ColorCasa{
            BLUE = 21,
            RED = 38,
            GREEN = 55,
            YELLOW = 4,
            LASTBLUEBOX = 17,
            LASTREDBOX = 34,
            LASTGREENBOX = 51,
            LASTYELLOWBOX = 68,
        };
};
#endif
