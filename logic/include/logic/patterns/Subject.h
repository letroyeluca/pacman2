//
// Created by Luca Letroye on 28/11/2025.
//

#ifndef PACMAN_SUBJECT_H
#define PACMAN_SUBJECT_H

#include "Observer.h"
#include <vector>
#include <memory>
#include <algorithm> // <--- Nodig voor std::remove (in detach)

namespace logic{
    class Subject{
    private:
        // VERANDERING 1: Vector bewaart nu gewone pointers, geen shared_ptrs
        std::vector<Observer*> m_observers;

    public:
        virtual ~Subject() = default;

        // VERANDERING 2: Accepteer een raw pointer
        void attach(Observer* observer) {
            m_observers.push_back(observer);
        }

        // VERANDERING 3: Zoek en verwijder de raw pointer
        void detach(Observer* observer){
            // Dit is de veilige standaardmanier om iets uit een vector te verwijderen
            auto it = std::remove(m_observers.begin(), m_observers.end(), observer);
            m_observers.erase(it, m_observers.end());
        }

    protected:
        void notify(Event event){
            // VERANDERING 4: Loop over raw pointers
            for(Observer* observer : m_observers){
                if (observer != nullptr) {
                    observer->onNotify(*this, event);
                }
            }
        }
    };
}

#endif //PACMAN_SUBJECT_H