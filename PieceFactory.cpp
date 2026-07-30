#include "PieceFactory.hpp"

#include "Pawn.hpp"
#include "ActivePawn.hpp"
#include "Rook.hpp"
#include "ActiveRook.hpp"
#include "Knight.hpp"
#include "Bishop.hpp"
#include "ActiveBishop.hpp"
#include "Queen.hpp"
#include "ActiveQueen.hpp"
#include "King.hpp"
#include "ActiveKing.hpp"

std::unique_ptr<Piece> PieceFactory::makePiece(const std::string& id, PlayerColor owner_color, Coordinates coordinates)
{
  try
  {
    if(id == "P")
      return std::make_unique<BasicPawn>(owner_color, coordinates);
    
    if(id == "PGLD")
      return std::make_unique<GoldenPawn>(owner_color, coordinates);
    
    if(id == "PIPT")
      return std::make_unique<ImpatientPawn>(owner_color, coordinates);
    
    if(id == "PSTB")
      return std::make_unique<StubbornPawn>(owner_color, coordinates);
    
    if(id == "PNRV")
      return std::make_unique<NervousPawn>(owner_color, coordinates);
    
    if(id == "PEXP")
      return std::make_unique<ExplosivePawn>(owner_color, coordinates);
    
    if(id == "R")
      return std::make_unique<BasicRook>(owner_color, coordinates);
    
    if(id == "RINV")
      return std::make_unique<InvincibleRook>(owner_color, coordinates);
    
    if(id == "RPNT")
      return std::make_unique<PainterRook>(owner_color, coordinates);
    
    if(id == "N")
      return std::make_unique<BasicKnight>(owner_color, coordinates);
    
    if(id == "NJMP")
      return std::make_unique<JumpyKnight>(owner_color, coordinates);
    
    if(id == "NICE")
      return std::make_unique<IceKnight>(owner_color, coordinates);
    
    if(id == "B")
      return std::make_unique<BasicBishop>(owner_color, coordinates);
    
    if(id == "BCLR")
      return std::make_unique<ColorBlindBishop>(owner_color, coordinates);
    
    if(id == "BPRC")
      return std::make_unique<PreacherBishop>(owner_color, coordinates);
    
    if(id == "Q")
      return std::make_unique<BasicQueen>(owner_color, coordinates);
    
    if(id == "QFLP")
      return std::make_unique<FlipperQueen>(owner_color, coordinates);
    
    if(id == "QJMP")
      return std::make_unique<JumpyQueen>(owner_color, coordinates);
    
    if(id == "QHNGR")
      return std::make_unique<HungryQueen>(owner_color, coordinates);
    
    if(id == "K")
      return std::make_unique<BasicKing>(owner_color, coordinates);
    
    if(id == "KFRT")
      return std::make_unique<FrightenedKing>(owner_color, coordinates);
    
    if(id == "KARC")
      return std::make_unique<ArcherKing>(owner_color, coordinates);
    
    return nullptr;
  }
  catch(std::bad_alloc& exception)
  {
    throw;
  }
}
