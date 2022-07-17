#ifndef URPLUSPLUS_TILEBASIC_H_
#define URPLUSPLUS_TILEBASIC_H_
#include "tile.h"

class TileBasic : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileBasic();
};

#endif
