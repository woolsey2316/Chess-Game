#include "global.h"
const double g_chessboardWidth = 550;
const double g_chessboardHeight = 550;
const double g_squareSize = (int) 1.0*g_chessboardHeight/8;
const int g_pixel_dx = 15;
const int g_pixel_dy = 15;
const std::string board_[8][8] = {
      {"black_rook","black_knight", "black_bishop", "black_queen", "black_king", "black_bishop","black_knight","black_rook"},
      {"black_pawn", "black_pawn" , "black_pawn"  , "black_pawn" , "black_pawn" ,  "black_pawn" , "black_pawn" ,"black_pawn"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {  "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,    "empty"   ,     "empty"   ,    "empty"   ,     "empty"},
      {"white_pawn", "white_pawn" , "white_pawn"  , "white_pawn" , "white_pawn" ,  "white_pawn" , "white_pawn" ,"white_pawn"},
      {"white_rook","white_knight", "white_bishop", "white_queen", "white_king" , "white_bishop","white_knight","white_rook"}
      };
