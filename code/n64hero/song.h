#include <vector>
#include <string>
#include <queue>

using namespace std;

typedef struct
{
    float time;
    float duration;
} Note;

typedef struct {
    string name;
    std::vector<vector<Note>> tracks;
} Song;

class SongTracker {
    private:
        std::vector<int> track_idx;
        std::vector<int> track_jdx;
        std::vector<vector<Note>> tracks;
    public:
        float elapsed;
        std::vector<deque<Note>> current_notes;

        void reset();
        void tick(float delta);
        
        SongTracker(Song song);
};