#include <vector>
#include <string>
#include <queue>

using namespace std;

typedef struct
{
    int time;
    int duration;
} Note;

typedef struct {
    string name;
    std::vector<vector<Note>> tracks;
} Song;

class SongTracker {
    private:
        float elapsed;
        std::vector<int> track_idx;
        std::vector<int> track_jdx;
        std::vector<vector<Note>> tracks;
    public:
        std::vector<deque<Note>> current_notes;

        void reset();
        void tick(float delta);
        
        SongTracker(Song song);
};