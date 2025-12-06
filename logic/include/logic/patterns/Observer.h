//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_OBSERVER_H
#define PACMAN_OBSERVER_H

#include "logic/Event.h"

namespace logic {
class Subject;

class Observer {
public:
  virtual ~Observer() = default;
  virtual void onNotify(const Subject &subject, Event event) = 0;
};
} // namespace logic

#endif // PACMAN_OBSERVER_H
