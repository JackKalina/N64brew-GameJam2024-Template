#include <string>
#include <vector>

typedef struct {
    int time;
    int duration;
} Note;

typedef struct {
    std::string name;
    std::vector<Note> track_cu;
    std::vector<Note> track_cr;
    std::vector<Note> track_cd;
    std::vector<Note> track_cl;
} Song;