#ifndef URPLUSPLUS_TILEROSETTE_H_
#define URPLUSPLUS_TILEROSETTE_H_
#include "tile.h"

class TileRosette : public Tile {
    bool specialCheck() override;
    bool doOnMoveSuccess(Token*, const std::vector<Tile*> &path) override;
  public:
    void acceptVisitor(GameViewer&) const override;
    TileRosette(size_t row, size_t col);
};

#endif
