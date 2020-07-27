#include <iostream>
#include <stdio.h>

using namespace std;

enum class Animal { dog, cat, bird  };

#ifndef TEMPLATE
class Sound {
public:
    void getSound ( const Animal& arg ) {
        switch ( arg ) {
            case Animal::dog:
                cout <<__PRETTY_FUNCTION__<<" This is a dog case!" << endl;
            break;

            case Animal::cat:
                cout <<__PRETTY_FUNCTION__<<" This is a cat case!" << endl;
            break;

            case Animal::bird:
                cout <<__PRETTY_FUNCTION__<<" This is a bird case!" << endl;
            break;

            default:
            return;
        }
    }
};
#endif

#ifdef TEMPLATE
class Sound {
   public:
      template<Animal animal>
      void getSound ();
};

template<>
void Sound::getSound<Animal::dog> () {
    cout <<__PRETTY_FUNCTION__<<" This is a dog case!" << endl;
}

template<>
void Sound::getSound<Animal::cat> () {
    cout <<__PRETTY_FUNCTION__<<" This is a cat case!" << endl;
}

template<>
void Sound::getSound<Animal::bird> () {
    cout <<__PRETTY_FUNCTION__<<" This is a bird case!" << endl;
}
#endif

int main() {
    Sound s;
    #ifdef TEMPLATE
    s.getSound<Animal::bird>();
    #else
    s.getSound(Animal::bird);
    #endif
}