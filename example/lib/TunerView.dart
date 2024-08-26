import 'dart:ffi';
import 'dart:io';

import 'package:ffi/ffi.dart';
import 'package:flutter/material.dart';
import 'package:audio_wave/audio_wave.dart';
import 'package:path_provider/path_provider.dart';

import 'package:sk_guitar_tuner/sk_guitar_tuner.dart' as sk_guitar_tuner;
import 'package:sk_guitar_tuner/SoundData.dart';

class TunerView extends StatefulWidget {
  const TunerView({super.key});

  @override
  _TunerViewState createState() => _TunerViewState();
}

class _TunerViewState extends State<TunerView> with SingleTickerProviderStateMixin {
  String info = "No information about frequencies.";
  String dirPath = "";

  bool _isPlaying = false;

  void _toggleAnimation() {
    setState(() {
      _isPlaying = !_isPlaying;
    });
  }

  late AnimationController _animationController = AnimationController(
    vsync: this,
    duration: Duration(milliseconds: 500), // Animation duration
  );

  late Animation<Color?> _colorAnimation = ColorTween(
    begin: Colors.blue, // Initial color
    end: Colors.red, // Target color
  ).animate(_animationController);

  @override
  void initState() {
    super.initState();
    createDirectory();
  }

  void createDirectory() async{
    final Directory extDir = await getApplicationDocumentsDirectory();
    dirPath = '${extDir.path}/Sound';
    await Directory(dirPath).create(recursive: true);
  }

  @override
  void dispose() {
    _animationController.dispose();

    super.dispose();
  }

  void _recordSound() {
    print('Record sound from flutter');
    Pointer<SoundData> soundData = sk_guitar_tuner.allocateSoundData();
  Pointer<Utf8> nativeDirPath = dirPath.toNativeUtf8();
    sk_guitar_tuner.startAudioRecorder(soundData, nativeDirPath);

    //
    // info = "Frequencies: ";
    // if (soundData.ref.frequencies == nullptr) {
    //   print("NULL PTR");
    // }
    //
    // final frequenciesList = soundData.ref.frequencies.asTypedList(soundData.ref.frequencySize);
    //
    // for (int i = 0; i < frequenciesList.length; i++) {
    //   print('Frequency $i: ${frequenciesList[i]}, ');
    //   info += '${frequenciesList[i]}, ';
    // }

    sk_guitar_tuner.stopAudioRecorder();

    sk_guitar_tuner.freeSoundData(soundData);
    print('Record sound from flutter');
    malloc.free(nativeDirPath);
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('SK Guitar Tuner'),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          children: <Widget>[
            Image.asset(
              'assets/tuner.jpg', // Replace with your image paths
              fit: BoxFit.contain,
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: () {
                _toggleAnimation();
                _recordSound();
              },
              style: ButtonStyle(
                backgroundColor: MaterialStateProperty.resolveWith<Color?>(
                      (Set<MaterialState> states) {
                    if (states.contains(MaterialState.pressed)) {
                      return Colors.red; // Color when pressed
                    }
                    return Colors.lightBlueAccent; // Default color
                  },
                ),
              ),
              child: Text('Record sound'),
            ),
            const SizedBox(height: 20),
            if (_isPlaying)
              AudioWave(
                height: 32,
                width: 32,
                spacing: 2,
                animationLoop: 3,
                bars: [
                  AudioWaveBar(
                      heightFactor: 0.2, color: Colors.lightBlueAccent),
                  AudioWaveBar(heightFactor: 0.4, color: Colors.lightBlue),
                  AudioWaveBar(heightFactor: 0.8, color: Colors.blue),
                  AudioWaveBar(heightFactor: 1, color: Colors.black),
                  AudioWaveBar(heightFactor: 0.8, color: Colors.blue),
                  AudioWaveBar(heightFactor: 0.4, color: Colors.lightBlue),
                  AudioWaveBar(
                      heightFactor: 0.2, color: Colors.lightBlueAccent),
                ],
              ),
          ],
        ),
      ),
    );
  }
}