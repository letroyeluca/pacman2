//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_SUBJECT_H
#define PACMAN_SUBJECT_H

#include "Observer.h"
#include <algorithm> // <--- Nodig voor std::remove (in detach)
#include <memory>
#include <vector>

namespace logic {
class Subject {
private:
    // VERANDERING 1: Vector bewaart nu gewone pointers, geen shared_ptrs
    std::vector<std::weak_ptr<Observer>> m_observers;

public:
    virtual ~Subject() = default;

    // VERANDERING 2: Accepteer een raw pointer
    void attach(std::shared_ptr<Observer> observer) {
        m_observers.push_back(observer);
    }

    // VERANDERING 3: Zoek en verwijder de raw pointer
    void detach(std::shared_ptr<Observer> observer) {
        // Verwijder specifieke observer (locken is nodig om te vergelijken)
        m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(),
                                         [&](const std::weak_ptr<Observer>& wp) {
                                             auto sp = wp.lock();
                                             return !sp || sp == observer;
                                         }),
                          m_observers.end());
    }

protected:
    void notify(Event event) {
        // Loop over observers en verwijder verlopen pointers direct (cleanup)
        auto it = m_observers.begin();
        while (it != m_observers.end()) {
            if (auto sp = it->lock()) { // Converteer weak naar shared
                sp->onNotify(*this, event);
                ++it;
            } else {
                // De observer bestaat niet meer, verwijder uit lijst
                it = m_observers.erase(it);
            }
        }
    }
};
} // namespace logic

#endif // PACMAN_SUBJECT_H