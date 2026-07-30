#include "MoveCommand.hpp"
#include "Coordinates.hpp"
#include "Square.hpp"
#include "Board.hpp"
#include "Utils.hpp"
#include "PieceFactory.hpp"
#include <cctype>
#include <iostream> 
//from Claude
// begin
bool MoveCommand::isValidSquare(const std::string& s) const
{
    if (s.length() != 2) return false;
    return (s[0] >= 'a' && s[0] <= 'h') && (s[1] >= '1' && s[1] <= '8');
}

bool MoveCommand::isValidPieceType(char c) const
{
    return c == 'r' || c == 'n' || c == 'b' || c == 'q' || c == 'k';
}

bool MoveCommand::isValidPromotionPiece(char c) const
{
    return c == 'r' || c == 'n' || c == 'b' || c == 'q';
}

void MoveCommand::parseMove(const std::string& s)
{
    std::string main = s;
    size_t eq_pos = s.find('=');
    if (eq_pos != std::string::npos)
    {
        is_promotion_ = true;
        std::string promo = s.substr(eq_pos + 1);
        if (promo.length() != 1 || !isValidPromotionPiece(promo[0]))
        {
            move_valid_ = false;
            return;
        }
        promotion_piece_ = promo;
        main = s.substr(0, eq_pos);
    }

    size_t x_pos = main.find('x');
    if (x_pos != std::string::npos)
    {
        is_capture_ = true;
        std::string before_x = main.substr(0, x_pos);
        std::string after_x  = main.substr(x_pos + 1);

        if (!isValidSquare(after_x))
        {
            move_valid_ = false;
            return;
        }
        target_square_ = after_x;

       if (before_x.length() == 1 && isValidPieceType(before_x[0]))
        {
            piece_type_ = before_x;
            if (before_x[0] >= 'a' && before_x[0] <= 'h')
            {
                is_pawn_move_ = true;
                pawn_file_ = before_x[0];
            }
        }
        else if (before_x.length() == 1 && before_x[0] >= 'a' && before_x[0] <= 'h')
        {
            is_pawn_move_ = true;
            pawn_file_ = before_x[0];
        }
        else if (before_x.length() == 2 && isValidPieceType(before_x[0]))
        {
            piece_type_ = before_x.substr(0, 1);
        }
        else
        {
            move_valid_ = false;
            return;
        }
    }
    else
    {
        if (!parseNonCapture(main)) { move_valid_ = false; return; }
    }

    if (is_promotion_ && !is_pawn_move_)
    {
        move_valid_ = false;
        return;
    }
}

bool MoveCommand::parseNonCapture(const std::string& main)
{
    if (main.length() == 2)
    {
        if (!isValidSquare(main)) return false;
        is_pawn_move_ = true;
        target_square_ = main;
        return true;
    }
    if (main.length() == 3)
    {
        if (!isValidPieceType(main[0]) || !isValidSquare(main.substr(1))) return false;
        piece_type_ = main.substr(0, 1);
        target_square_ = main.substr(1);
        return true;
    }
    return false;
}
//end


// NE DIRAJ, FUNCIJA RADI, AKO TI NESTO NIJE JASNO PITAJ
CommandReturnType MoveCommand::execute(Board& board, Player& active_player, Player& opponent_player)
{
    CommandReturnType status = valdateAndParse();
    if(status != CommandReturnType ::ACTIVE_SUCCESS)
    {
        return status;
    }

    Coordinates target_coordinates = Coordinates(target_square_);
    Square* target_square = board.getSquare(target_coordinates);
    Piece* piece = nullptr;

    status = findPiece(board, active_player, target_coordinates, piece);
    if(status != CommandReturnType ::ACTIVE_SUCCESS)
    {
        return status;
    }
    handleEnPassant(board, active_player, target_coordinates, piece);

    bool is_castling = false;
    if(piece_type_  == "k" && !is_capture_ && std::abs(static_cast<int>(piece->getCoordinates().getFile()) - 
        static_cast<int>(target_coordinates.getFile())) == 2 && piece->getCaslintgRook() != nullptr)
    {
        is_castling = true;
    }

   status = checkTargetSquare(board, active_player, opponent_player, piece, target_square, is_castling);
   if(status != CommandReturnType ::ACTIVE_SUCCESS)
    {
        return status;
    }
    if(blocked_)
    {
        return CommandReturnType::ACTIVE_SUCCESS;
    }
    if(is_castling)
    {
        CommandReturnType status = castling(board, opponent_player, piece, target_square);
        if (status != CommandReturnType::ACTIVE_SUCCESS)
        return status;
    }

    status = handleMove(board, active_player, opponent_player, piece, target_square);
    if(status != CommandReturnType ::ACTIVE_SUCCESS)
    {
        return status;
    }
    if(is_promotion_)
    {   
        if(piece->getId() == PieceID::PGLD)
        {
            return CommandReturnType::WIN;
        }
        promotion(board, active_player, piece, target_coordinates);
    }

    for( auto& piece : opponent_player.getPrisonPieces())
    {
        if(piece.get()->getPieceType() == PieceType::K)
        {
            return CommandReturnType::WIN;
        }
    }
    return CommandReturnType::ACTIVE_SUCCESS;
}


CommandReturnType MoveCommand::valdateAndParse()
{
    if (parameters_.size() != 1)
    {
        printErrorMessage("E_INVALID_PARAM_COUNT");
        return CommandReturnType::ERROR;
    }

    std::string parameter = parameters_[0];

    parseMove(parameter);

    if (!move_valid_)
    {
        printErrorMessage("E_INV_PARAM_MOVE");
        return CommandReturnType::ERROR;
    }
    return CommandReturnType::ACTIVE_SUCCESS;
}

CommandReturnType MoveCommand::findPiece(Board& board, Player& active_player,Coordinates target_coordinates, 
    Piece*& piece)
{
    std::vector<Piece*> potential_pieces;
    //from claude
    //begin
    if (is_pawn_move_)
    {
        auto pawn_pieces = getPotentialPiecesMove(board, active_player, target_coordinates,
            "p", is_capture_, pawn_file_);
        potential_pieces.insert(potential_pieces.end(), pawn_pieces.begin(), pawn_pieces.end());
    }
    if (!piece_type_.empty())
    {
        auto piece_pieces = getPotentialPiecesMove(board, active_player, target_coordinates,
            piece_type_, is_capture_, '\0');
        potential_pieces.insert(potential_pieces.end(), piece_pieces.begin(), piece_pieces.end());
    }
    //end
    if(is_pawn_move_)
    {
        piece_type_ = "p";
    }
    
    if (potential_pieces.empty())
    {
        printErrorMessage("E_INVALID_MOVE");
        return CommandReturnType::ERROR;
    }
    piece = potential_pieces[0];
    if(potential_pieces.size() > 1)
    {
       CommandReturnType status = ambiguousMove(potential_pieces, active_player, piece);
       if(status != CommandReturnType::ACTIVE_SUCCESS)
       {
            return status;
       }
    }

    piece->setTargetSquare(board.getSquare(target_coordinates));

    if(active_player.getNumberOfMoves() > 0)
    {
        if(piece != active_player.getLastMovedPiece())
        {
            printErrorMessage("E_INVALID_MOVE");
            return CommandReturnType::ERROR;
        }
    }


    if(piece->getId() == PieceID::NICE ) // ice knight
    {
        piece->special(&board, nullptr);
    }
    
    if(!is_promotion_ && is_pawn_move_ &&((active_player.getColor() == PlayerColor::WHITE 
        && target_coordinates.getRank() == 7) || (active_player.getColor() == PlayerColor::BLACK 
        && target_coordinates.getRank() == 0)))
    {
        printErrorMessage("E_INVALID_MOVE");
        return CommandReturnType::ERROR;
    }
    return CommandReturnType::ACTIVE_SUCCESS;
}

void MoveCommand::handleEnPassant(Board& board, Player& active_player, Coordinates target_coordinates, Piece* piece)
{
    if(piece->getMovingEnPassant())
    {
        if(active_player.getColor() == PlayerColor::WHITE)
        {
            en_passant_pawn_ = board.getSquare(Coordinates(target_coordinates.getFile(), 4))->getPiece();
        }
        else
        {
            en_passant_pawn_ = board.getSquare(Coordinates(target_coordinates.getFile(), 3))->getPiece();
        }
    }
}

CommandReturnType MoveCommand::checkTargetSquare(Board& board, Player& active_player,Player& opponent_player, 
Piece* piece, Square* target_square, bool is_castling)
{
    if(is_capture_)
    {
       CommandReturnType status = capture(board, active_player, opponent_player,
        piece, target_square);
        if (status != CommandReturnType::ACTIVE_SUCCESS)
        {
            return status;
        }
        if (blocked_)
        {
            return CommandReturnType::ACTIVE_SUCCESS;
        }
    }
    else
    {
        if(target_square->getPiece() != nullptr && !piece->getMovingEnPassant() && !is_castling )
        {
            printErrorMessage("E_INVALID_MOVE");
            return CommandReturnType::ERROR;
        }
    }

    if(piece->getId() == PieceID::KFRT && piece->isInCheck(&opponent_player, &board))
    {
        active_player.setCanMoveMultiple(true);
        active_player.setLastMovedPiece(piece);
    } 

    if(active_player.checkForCheckmate(&opponent_player, &board))
    {
        return CommandReturnType::WIN;
    }
    return CommandReturnType::ACTIVE_SUCCESS;
}

CommandReturnType MoveCommand::handleMove(Board& board, Player& active_player,Player& opponent_player, Piece* piece,
Square* target_square)
{
    if(opponent_player.getLastMovedPiece() != nullptr)
    {
        opponent_player.getLastMovedPiece()->setPawnMovedTwoSqures(false);
    }

    Coordinates old_coordinates = piece->getCoordinates();
    board.getSquare(piece->getCoordinates())->removePiece();
    board.placePiece(target_square->getCoordinates(), piece);
    piece->setHasMoved(true);
    active_player.setLastMovedPiece(piece);
    piece->setMovingEnPassant(false);

    if(piece_type_ == "p" && !is_capture_ && std::abs(static_cast<int>(old_coordinates.getRank()) - 
        static_cast<int>(target_square->getCoordinates().getRank())) == 2)
    {
       piece->setPawnMovedTwoSqures(true);

    }

    // Move history
    auto move = std::make_unique<Move>(board.getRound(),active_player.getColor(), parameters_[0] );
    board.addToMoveHistory(std::move(move));

    if(target_square->getItem() != nullptr)
    {
        auto item = target_square->getItem();
        target_square->removeItem();
        piece->setItem(item);
    }

    if(target_square->getSquareType() == SquareType::BOOST)
    {
        active_player.setLastMovedPiece(piece);
        active_player.setCanMoveMultiple(true);
    }

    if(target_square->getSquareType() == SquareType::MANA)
    {
        active_player.setMana(active_player.getMana() + 1);
    }
    return CommandReturnType::ACTIVE_SUCCESS;
}


CommandReturnType MoveCommand::ambiguousMove(const std::vector<Piece*>& potential_pieces, Player& active_player,
Piece*& piece)
{
    printMessage("D_AMBIGUOUS_MOVE");
    std::cout <<active_player.getID()<<" > ";
    std::string input;
    std::getline(std::cin, input);
    Utils::toLowerCase(input);
    bool is_trimmed = true;
    Utils::trim(input);
    std::vector<std::string> user_input = Utils::splitString(input, ' ', is_trimmed);
    if(user_input[0] == "quit" && user_input.size() == 1)
    {
        return CommandReturnType::QUIT;
    }
    else if(user_input[0] == "cancel" && user_input.size() == 1)
    {
        return CommandReturnType::ERROR;
    }
    else if(user_input[0].size() == 2 && user_input[0][0] >= 'a' && user_input[0][0] <= 'h' 
    && user_input[0][1] >='1' && user_input[0][1] <= '8')
    {
        bool found = false;
        Coordinates target_coordinates = Coordinates(user_input[0]);
        for(auto final_piece : potential_pieces)
        {
            if(final_piece->getCoordinates().getFile() == target_coordinates.getFile()
                && final_piece->getCoordinates().getRank() == target_coordinates.getRank())
            {
                piece = final_piece;
                return CommandReturnType::ACTIVE_SUCCESS;
            }
        }
        if(!found)
        {
            printErrorMessage("E_INVALID_MOVE");
            return CommandReturnType::ERROR;
        }
    }
    else
    {
        printErrorMessage("E_INV_PARAM_SQUARE");
        return CommandReturnType::ERROR;
    }
    return CommandReturnType::ERROR;
}



CommandReturnType MoveCommand::capture(Board& board, Player& active_player, Player& opponent_player, Piece* piece, 
Square* target_square)
{
    Piece* captured = target_square->getPiece();

    if(en_passant_pawn_ != nullptr)
    {
        captured = en_passant_pawn_;
    }
   
    if (captured == nullptr || captured->getInvincible() != -1)
    {
        printErrorMessage("E_INVALID_MOVE");
        return CommandReturnType::ERROR;
    }
    if (captured->getColor() == active_player.getColor() && piece->getId() != PieceID::QHNGR)
    {
        printErrorMessage("E_INVALID_MOVE");
        return CommandReturnType::ERROR;
    }
    if(captured->hasItem("SHIELD"))
    {
        Item* shield_tool = captured->getItem();
        shield_tool->triggerEffect(board, target_square, nullptr, "");
        blocked_ = true;
        return CommandReturnType::ACTIVE_SUCCESS;
    }
    if(captured->hasItem("REPEL") && piece->getPieceType() == PieceType::Q)
    {
        printErrorMessage("E_INVALID_MOVE");
        return CommandReturnType::ERROR;
    }
    
    if(piece->getId() == PieceID::KFRT && piece->isInCheck(&opponent_player, &board))
    {
        printErrorMessage("E_INVALID_MOVE");
        return CommandReturnType::ERROR;
    }

    if(opponent_player.getLastMovedPiece() == captured)
    {
        opponent_player.setLastMovedPiece(nullptr);
    }

    captured->sendToPrison();

    if(captured->getItem() != nullptr)
    {
        piece->setItem(captured->getItem());
        captured->removeItem();
    }
    if(piece->getId() == PieceID::QHNGR && piece->getColor() == captured->getColor() )
    {
        active_player.addToMyPrison(captured, &opponent_player);
    }
    else
    {
        captured->getOwner()->addToPrison();
    }
   if(en_passant_pawn_ != nullptr)
    {
        board.getSquare(captured->getCoordinates())->removePiece();
    }

    return CommandReturnType::ACTIVE_SUCCESS;
}

void MoveCommand::promotion(Board& board, Player& active_player, Piece* piece,
    const Coordinates& target_coordinates)
{
    std::string id(1, std::toupper(static_cast<unsigned char>(promotion_piece_[0])));
    std::unique_ptr<Piece> new_piece = PieceFactory::makePiece(id, active_player.getColor(), target_coordinates);
    std::vector<std::unique_ptr<Piece>>& pieces = active_player.getPieces();
    for(auto& current : pieces)
    {
        if(current.get() == piece)
        {
            board.getSquare(target_coordinates)->removePiece();
            board.placePiece(target_coordinates, new_piece.get());
            new_piece->setOwner(&active_player);
            current = std::move(new_piece);
            break;
        }
    }
}

CommandReturnType MoveCommand::castling(Board& board, Player& opponent_player,
    Piece* piece, Square* target_square)
{
    if (!piece->isInCheck(&opponent_player, &board))
    {
        
        Piece* rook = piece->getCaslintgRook();
        
        Coordinates new_rook_coordinates = rook->getCoordinates();

        if(target_square->getCoordinates().getFile() > piece->getCoordinates().getFile())
        {
            for(std::size_t i = piece->getCoordinates().getFile() + 1;  i <= 
                target_square->getCoordinates().getFile(); i++)
            {
                for(auto& opp_piece : opponent_player.getPieces())
                {
                    if(opp_piece->canMoveToTargetSquare(board, Coordinates(i, piece->getCoordinates().getRank())
                        , false))
                    {
                        printErrorMessage("E_INVALID_MOVE");
                        return CommandReturnType::ERROR;
                    }
                }
            }
        }
        else
        {
            for(int i =static_cast<int> (piece->getCoordinates().getFile()) -1 ;
                i >= static_cast<int>(target_square->getCoordinates().getFile()); i--)
            {
                    for(auto& opp_piece : opponent_player.getPieces())
                {
                    if(opp_piece->canMoveToTargetSquare(board, Coordinates(i, piece->getCoordinates().getRank())
                        , false))
                    {
                        printErrorMessage("E_INVALID_MOVE");
                        return CommandReturnType::ERROR;
                    }
                }
            }
        }
        if(target_square->getCoordinates().getFile() > piece->getCoordinates().getFile())
        {
            new_rook_coordinates.setFile(target_square->getCoordinates().getFile() - 1);
        }
        else
        {
            new_rook_coordinates.setFile(target_square->getCoordinates().getFile() + 1);
        }

        board.getSquare(rook->getCoordinates())->removePiece();
        board.placePiece(new_rook_coordinates, rook);
        rook->setHasMoved(true);
        piece->setCastlingRook(nullptr);
    }
    else
    {
        return CommandReturnType::ERROR;
    }
    return CommandReturnType::ACTIVE_SUCCESS;
}

std::vector<Piece*> MoveCommand::getPotentialPiecesMove(Board& board, Player& active_player,
    Coordinates target, std::string type, bool is_capture,  char pawn_file)
{
    std::vector<Piece*> potential_pieces;
    for(auto& piece : active_player.getPieces())
    {
        if(piece->getTypeString() == type && piece->canMoveToTargetSquare(board, target,is_capture)
            && piece->isFrozen() == false && !piece->isEvenOddTriggered(board.getRound()))
        // treba da provreim da li je na target square friendly piece
        {
            if(type == "p" && is_capture && pawn_file != '\0')
            {
                if(piece->getCoordinates().getFile() != (size_t)(pawn_file - 'a'))
                {
                    continue;
                }
            }
            potential_pieces.push_back(piece.get());
        }
    }
    return potential_pieces;
}

