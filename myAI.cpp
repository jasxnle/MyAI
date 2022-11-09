#include "MyOthelloAI.hpp"
#include <ics46/factory/DynamicFactory.hpp>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, lejb1::MyOthelloAI, "Jason Le's AI (Required)");

std::pair<int,int> lejb1::MyOthelloAI::chooseMove(const OthelloGameState& state)
{
    bool color = true; // false will be for white, true will be for black

    //if turn is black turn
    if(state.isBlackTurn())
    {
        color = false;
    }

    //temp best val
    int bestVal = -1000;
    //find the move we want to do
    int index =0;

    //generate possible moves
    std::vector<std::pair<int,int>> moves = findPossMoves(state);

    for( unsigned int i = 0; i < moves.size(); i++ )
    {
        //perform move on clon
        std::unique_ptr<OthelloGameState> clone = state.clone();
        clone->makeMove(moves[i].first, moves[i].second);

        //do search
        int tempVal = search(*clone, 3, color);

        //if it is the best move then replace 
        if(tempVal>bestVal)
        {
            //resetting comparison
            bestVal = tempVal;
            index = i;
        }
            

    }

    //return pair
    return moves[index];

}

//do return type pair
int lejb1::MyOthelloAI::search(const OthelloGameState& s, int depth, bool color) 
{
    //finding the current color from the game state
    bool color2 = true; //flag true means white by default
    if (s.isBlackTurn())
        color2 = false; //flag false means black if needed

    std::vector<std::pair<int,int>> possMoves = findPossMoves(s);

    //base case
    if(depth == 0 || s.isGameOver() || possMoves.size() == 0)
    {
        return eval(s, color);
    }
    else
    {   
        
        //this is my turn (max)
        if(color == color2)
        {
            int max = 0;
            //for each possible move try and see which move is the best
            for(unsigned int i = 0; i < possMoves.size(); ++i)
            {
                //making a clone of game in order to test the move
                std::unique_ptr<OthelloGameState> moveTest = s.clone();
                moveTest->makeMove(possMoves[i].first, possMoves[i].second);

                //recursive call
                int tempMax = search(*moveTest, depth-1, color);

                //setting max return value
                if(tempMax > max)
                    max = tempMax;
                
            }

            return max;
        }
        else //opponents turn (min)
        {
            int min = 0;
            //for each possible move try and see which move is the best
            for(unsigned int i = 0; i < possMoves.size(); ++i)
            {
                //making a clone of game in order to test the move
                std::unique_ptr<OthelloGameState> moveTest = s.clone();
                moveTest->makeMove(possMoves[i].first, possMoves[i].second);

                //recursive call
                int tempMin = search(*moveTest, depth-1, color);

                //setting max return value
                if(tempMin < min)
                    min = tempMin;
                
            }
            return min;
        }

    }
}

std::vector<std::pair<int,int>> lejb1::MyOthelloAI::findPossMoves(const OthelloGameState& state)
{
    //possMoves will be a vector that holds the possible moves
    std::vector<std::pair<int,int>> possMoves;

    //making a board reference in order to acess the member functions
    const OthelloBoard& boardState = state.board();

    //loop through the whole board to see what possible move there are
    for( unsigned int i = 0; i < boardState.width(); i++) // i will be the columns
    {
        for(unsigned int j = 0; j < boardState.height(); j++) //j will be the rows
        {
            //if is validCell to move to move the possible moves back into the vector
            if(state.isValidMove(i,j))
            {
                possMoves.push_back(std::make_pair(i,j));
            }
        }
    }

    return possMoves;
}

int lejb1::MyOthelloAI::eval(const OthelloGameState& state, bool color)
{
    // if color is false means it is black turn
    //if color is true means it is white turn
    // simple eval function

    //if black turn do this
    if(color == false)
    {
        return state.blackScore() - state.whiteScore();
    }
    else //if white turn do this
    {
        return state.whiteScore()-state.blackScore();
    }
}
