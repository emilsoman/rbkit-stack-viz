#pragma once

class World {
  public:
    float referenceTime;
    int scale;
    World() {
      referenceTime = 0;
      scale = 1.0;
    }
};
