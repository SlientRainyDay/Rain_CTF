from mido import MidiFile
import mido as mido

chromatic_scale = {0: 'C', 1: 'C#', 2: 'D', 3: 'D#', 4: 'E', 5: 'F', 6: 'F#', 7: 'G', 8: 'G#', 9: 'A', 10: 'A#', 11: 'B'}

gmaj = {0: 'F', 1: 'G', 2: 'A', 3: 'B♭', 4: 'C', 5: 'D', 6: 'E'}


def select_notes_from_file(midi_file):
    midi_file = MidiFile(midi_file)
    track = mido.merge_tracks(midi_file.tracks)
    ticks_per_beat = midi_file.ticks_per_beat

    notes = []
    play_time = 0
    for msg in track:
        m = msg.dict()
        play_time += m.get('time')

        # each beat is a quarter note, we care about sixteenth notes
        sixteenth = (play_time % ticks_per_beat)/(ticks_per_beat/4)

        # Pick odd eighth notes
        if (m.get('type') == 'note_on'
        and m.get('velocity') != 0
        and (sixteenth % 2) != 0):
            notes.append(chromatic_scale.get(m.get('note') % 12))
    return notes

def gmaj_to_sept(note_list):
    gmaj_r = {v: k for k, v in gmaj.items()} # Reverse keys and values of gmaj dict
    final = []
    sept = ''
    for note in note_list:
        sept += str(gmaj_r.get(note))
        if len(sept) == 3:
            final.append(sept)
            sept = ''
    return final

def sept_to_dec(sept_list):
    final = []
    for sept in sept_list:
        result = 0
        digits = split(sept)
        power = 0
        while len(digits) != 0:
            result += int(digits.pop()) * 7 ** power
            power += 1
        final.append(result)
    return final

def dec_to_char(dec_list):
    final = []
    for dec in dec_list:
        final.append(chr(dec))
    return final

def split(word):
    return [char for char in word]

notes = select_notes_from_file('piano.mid')

# Decoding
print('Selected notes by rhythm : {}'.format(', '.join(notes)))

sept = gmaj_to_sept(notes)
print('Base 7, 3 digits groups  : {}'.format(', '.join(sept)))

decimal = sept_to_dec(sept)
print('Base 10 from Base 7      : {}'.format(', '.join(map(str, decimal))))

chars = dec_to_char(decimal)
print('ASCII TEXT               : {}'.format(''.join(chars)))
