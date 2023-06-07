#ifndef __AI_PLAYER_H__
#define __AI_PLAYER_H__

# include "Attributes.h"
# include "Player.h"

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
        double Heuristica1(const Parchis &estado, int jugador) const;
        double ContarDistancia(const Parchis &estado, color c) const;
        // Segundo Encuentro
        double Heuristica2(const Parchis &estado, int player) const;
        double minimax(Parchis &state, int depth, int player, color &best_piece, int &best_dice, bool maximizingPlayer) const;
        // Tercer Encuentro
        double Heuristica3(const Parchis &estado, color c, int player) const;
        bool isVulnerablePiece(const Parchis &estado, const Piece &piece, int player) const;
        double enemyDistance(const Parchis &estado, int player) const;
        bool isBeneficialToLeaveHome(const Parchis &estado, const Piece &piece, int player) const;
        bool isPieceInHome(const Piece &piece) const;
        bool isPieceOneSquareFromCorridor(const Piece &piece) const;
        bool clearPathBetweenTwoSquares(const Parchis &state, const Box &b1, const Box &b2, const Piece &piece) const; // Comprueba si se puede transitar desde b1 hasta b2
        Box calculateBoxType(const Piece &piece, const int positionIncrement) const;
        bool pieceCanBeEatenByRedShell(const Parchis &state, const Piece &piece, int enemyPlayer, const Piece &targetPiece) const;
        bool pieceCanBeEatenByBlueShell(const Parchis &state, const Piece &piece, int enemyPlayer, const Piece &targetPiece) const;
        double podaAlphaBeta(Parchis &state, int depth, int player, color &best_piece, int &best_dice, bool maximizingPlayer, double alpha, double beta) const;

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
