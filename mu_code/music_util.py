# Copyright (c) 2017 Xiaoyong Guo
# Author: xguo

import pyaudio
import numpy as np

class MusicScore(object):
  def __init__(self, simple_score):
    tokens = []
    token = ''
    for letter in simple_score:
      if ord('0') <= ord(letter) <= ord('8'):
        if token:
          tokens.append(token)
        token = letter
      elif letter in '._-=<>#':
        token += letter
    if token:
      tokens.append(token)
    self._tokens = tokens

  @staticmethod
  def parse_token(token):
    pitch_names = 'RCDEFGABC'
    note = {
      "pitch": 'R',
      "octave": 4,
      'duration': 4
    }
    note['name'] = pitch_names[int(token[0])]
    if note['name'] == 'R':
      note['octave'] = ''
    n = 1
    while n < len(token):
      letter = token[n]
      if letter == '<':
        note['octave'] -= 1
        n += 1
      elif letter == '>':
        note['octave'] += 1
        n += 1
      elif letter == '.':
        note['duration'] += (note['duration']//2)
        n += 1
      elif letter == '-':
        if token[n:n+3] == '---':
          note['duration'] *= 4
          n += 3
        elif token[n:n+2] == '--':
          note['duration'] *= 3
          n += 2
        else:
          note['duration'] *= 2
          n += 1
      elif letter == '_':
        note['duration'] //= 2
        n += 1
      elif letter == '=':
        note['duration'] //= 4
        n += 1
      elif letter == '#':
        note['pitch'] += '#'
        n += 1
      else:
        raise Exception();
    note = '%s%s:%s' % (note['name'], note['octave'], note['duration'])
    return note

  def translate(self):
    result = []
    for token in self._tokens:
      note = self.parse_token(token)
      result.append(note)
    return result


class MusicWaveGenerator(object):
  def __init__(self):
    A4 = 440.0  # scientific pitch frequency, the 69-th pitch
    pitch0_frequency = A4*(2**(-69/12.0))
    pitch_names = [
        'C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
    pitch_notations = [
        pitch + str(n) for n in range(-1, 10) for pitch in pitch_names]
    pitch_frequencies = [
        pitch0_frequency*(2**(n/12.0)) for n in range(len(pitch_notations))]
    self._pitches = dict(zip(pitch_notations, pitch_frequencies))
    self._fs = 44100
    self._speed = 2.

  def get_frequency(self, spn):
    return self._pitches[spn]

  def gen_wave_data(self, score):
    samples_list = []
    for note in score:
      pitch_notation, duration = note.split(':')
      if pitch_notation[0] == 'R':
        pitch_frequency = 1.0;
      else:
        pitch_frequency = self._pitches[pitch_notation]
      duration = float(duration)/4.0*self._speed
      samples_list.append(
          (np.sin(2*np.pi*np.arange(self._fs*duration)*
                  pitch_frequency/self._fs)).astype(np.float32))
    return samples_list

  def play(self, score):
    samples_list = self.gen_wave_data(score)
    audio = pyaudio.PyAudio()
    stream = audio.open(
        format=pyaudio.paFloat32, channels=1, rate=self._fs, output=True)
    for samples in samples_list:
      stream.write(samples)
    stream.stop_stream()
    stream.close()
    audio.terminate()
