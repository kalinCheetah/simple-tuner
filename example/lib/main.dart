import 'package:flutter/material.dart';
import 'dart:async';

import 'package:sk_guitar_tuner/sk_guitar_tuner.dart' as sk_guitar_tuner;
import 'package:sk_guitar_tuner_example/TunerView.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: TunerView(),
    );
  }
}