import 'package:flutter/material.dart';
import 'dart:async';

import 'package:sk_guitar_tuner/sk_guitar_tuner.dart' as sk_guitar_tuner;
import 'package:sk_guitar_tuner_example/TunerView.dart';
import 'package:permission_handler/permission_handler.dart' as permission_handler;

void main() async {
  WidgetsFlutterBinding.ensureInitialized(); // Initialize the Flutter binding

  var status = await permission_handler.Permission.microphone.status;
  if (!status.isGranted) {
    await permission_handler.Permission.microphone.request();
  }

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