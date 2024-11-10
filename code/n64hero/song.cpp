#include <vector>
#include <string>
#include <queue>
#include "song.h"

using namespace std;

SongTracker::SongTracker(Song song) {
    tracks = song.tracks;
    track_idx.reserve(tracks.size());
    current_notes.reserve(tracks.size());
}

void SongTracker::reset() {

}

void SongTracker::tick(float delta) {
    elapsed += delta;
    for (int n=0; n < (int)tracks.size(); n++) {

        if ((int)tracks[n].size() > track_idx[n] && elapsed + 3 >= tracks[n][track_idx[n]].time) {
            current_notes[n].push_back(tracks[n][track_idx[n]]);
        }


        if (tracks[n][track_idx[n]].time + .1 > elapsed) {
            track_idx[n]++;
        }

        // Check if notes in future in current view
        if ((int)tracks[n].size() > track_jdx[n] && elapsed + 3 >= tracks[n][track_jdx[n] + 1].time) {
            track_jdx[n]++;
        }
    }
}