#include <vector>
#include <string>
#include <queue>
#include "song.h"
#include <stdio.h>
#include <libdragon.h>

using namespace std;

SongTracker::SongTracker(Song song) {
    elapsed = -3;
    tracks = song.tracks;
    track_idx = std::vector<int>(tracks.size(), 0);
    track_jdx = std::vector<int>(tracks.size(), 0);
    current_notes.reserve(tracks.size());
    for (int i =0; i < (int)tracks.size(); i++) {
        current_notes.push_back(std::deque<Note>());
    }
}

void SongTracker::reset() {

}

void SongTracker::tick(float delta) {
    elapsed += delta;
    
    for (int n=0; n < (int)tracks.size(); n++) {

        if ((int)tracks[n].size() > track_idx[n] && elapsed + 5 >= tracks[n][track_idx[n]].time) {
            current_notes[n].push_back(tracks[n][track_idx[n]]);
            track_idx[n]++;
        }

        if (current_notes[n].size() > 0 && elapsed > current_notes[n].front().time + .5) {
            current_notes[n].pop_front();
        }
    }
}
