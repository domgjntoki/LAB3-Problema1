import numpy as np
import wave

def apply_tremolo(input_wavefile, output_wavefile, depth, frequency):
    # Carrega o arquivo de áudio
    wave_file = wave.open(input_wavefile, 'rb')
    params = wave_file.getparams()
    audio_frames = np.frombuffer(wave_file.readframes(params.nframes), dtype=np.int16)
    wave_file.close()

    # Gera a onda de tremolo
    time = np.arange(len(audio_frames)) / params.framerate
    tremolo_wave = 0.5 * (1.0 + np.sin(2.0 * np.pi * frequency * time))

    # Aplica o efeito de tremolo ao sinal de áudio
    tremolo_audio = audio_frames * tremolo_wave * depth

    # Normaliza o sinal resultante
    tremolo_audio = np.int16(tremolo_audio / np.max(np.abs(tremolo_audio)) * 32767)

    # Salva o novo arquivo de áudio
    with wave.open(output_wavefile, 'wb') as output_wave:
        output_wave.setparams(params)
        output_wave.writeframes(tremolo_audio.tobytes())

def apply_tremolo2(input_wavefile, output_wavefile2, amplitude, frequency):
    # Carrega o arquivo de áudio
    wave_file = wave.open(input_wavefile, 'rb')
    params = wave_file.getparams()
    audio_frames = np.frombuffer(wave_file.readframes(params.nframes), dtype=np.int16)
    wave_file.close()

    # Gera a onda de tremolo
    time = np.arange(len(audio_frames)) / params.framerate

    # Oscilador de modulação 
    m = np.sin(2.0 * np.pi * frequency * time)

    # Aplica o efeito de tremolo ao sinal de áudio
    tremolo_audio = (1 + amplitude * m) * audio_frames

    # Normaliza o sinal resultante
    tremolo_audio = np.int16(tremolo_audio / np.max(np.abs(tremolo_audio)) * 32767)

    # Salva o novo arquivo de áudio
    with wave.open(output_wavefile2, 'wb') as output_wave2:
        output_wave2.setparams(params)
        output_wave2.writeframes(tremolo_audio.tobytes())


# Parâmetros do efeito de tremolo
depth = 0.6  # Profundidade do tremolo (0.0 a 1.0)
frequency = 1  # Frequência do tremolo em Hz
amplitude = 10  # Amplitude da modulação

# Caminho do arquivo de áudio de entrada e saída
input_wavefile = 'python/Guitar_Sound/Guitar_Normal.wav'
output_wavefile = 'python/Guitar_Sound/Guitar_Tremolo.wav'
output_wavefile2 = 'python/Guitar_Sound/Guitar_Tremolo2.wav'

# Aplica o efeito de tremolo
apply_tremolo(input_wavefile, output_wavefile, depth, frequency)
apply_tremolo2(input_wavefile, output_wavefile2, amplitude, frequency)
