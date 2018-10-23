#ifndef CHESSPIECE_H
#define CHESSPIECE_H
#include <array>
#include <vector>
#include <string>

class ChessPiece {
 protected:
  std::string COLOUR;
  std::string type;
  bool hasMoved;
  int totalMoves;
  std::vector<std::array<int,2>> moveList;

 public:
	virtual bool isValidMove(int x, int y, int dest_x, int dest_y)=0;
	virtual bool isValidMove(int x, int y, int dest_x, int dest_y, bool isCapturing)=0;
  virtual bool isValidEnPassant(int start_x, int start_y, int dest_x, int dest_y, std::string pieceType, int* recentMove)=0;
  virtual bool isValidCapture(int start_x, int start_y, int dest_x, int dest_y, std::string COLOUR)=0;
  virtual bool isDefending(int start_x, int start_y, int dest_x, int dest_y, std::string COLOUR)=0;
  virtual std::vector<std::array<int, 2>> possibleMoves(int x_pos, int y_pos)=0;
	ChessPiece();
	ChessPiece(std::string clr, std::string name);
  std::string getType();
	bool capturesOwnColour();
	int * getPosition();
	std::string getColour();
	bool moved();
	int getTotalMoves();
	void setHasMoved(bool moved);
	void incrementTotalMoves();
	bool matingPiece();

};

#endif
