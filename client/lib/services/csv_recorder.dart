import 'dart:async';
import 'dart:io';

import 'package:path/path.dart' as p;
import 'package:sensor_data_app/models/sampled_value.dart';

/// CsvRecorder expects prepared samples to be forwarded to it via
/// `recordSample(sensorName, unit, sample)`.
class CsvRecorder {
  final String folderPath;
  final String baseFileName;

  IOSink? _sink;
  File? _file;
  String? _currentSensorColumnLabel;
  bool _started = false;

  CsvRecorder({required this.folderPath, String? baseFileName})
    : baseFileName = baseFileName ?? 'sensor_record';

  Future<void> start() async {
    final dir = Directory(folderPath);
    if (!await dir.exists()) {
      await dir.create(recursive: true);
    }
    _started = true;
  }

  Future<void> stop() async {
    _started = false;
    try {
      await _sink?.flush();
      await _sink?.close();
    } catch (_) {}
    _sink = null;
    _file = null;
    _currentSensorColumnLabel = null;
  }

  /// Record a prepared sample into the CSV. This method is safe to call even
  /// if the recorder hasn't been started; it will try to create the folder
  /// on demand. Rows are written immediately.
  Future<void> recordSample(
    String sensorName,
    String unit,
    SampledValue sample,
  ) async {
    if (!_started) {
      // Try to auto-start so callers don't have to manage ordering strictly
      await start();
    }

    final columnLabel = '$sensorName [$unit]';

    // If sensor/unit changed (or no file yet), rotate file
    if (_sink == null ||
        _currentSensorColumnLabel == null ||
        _currentSensorColumnLabel != columnLabel) {
      await _rotateFileWithHeader(columnLabel, sample.timestamp);
    }

    final unix = sample.timestamp.millisecondsSinceEpoch ~/ 1000;
    final values = <String>[];
    values.add(unix.toString());
    values.add(sample.value.toString());
    values.add(sample.sampleCount.toString());

    final escaped = values.map((v) => '"${v.replaceAll('"', '""')}"').join(',');
    _sink!.writeln(escaped);
    await _sink!.flush();
  }

  Future<void> _rotateFileWithHeader(String sensorColumn, DateTime now) async {
    try {
      await _sink?.flush();
      await _sink?.close();
    } catch (_) {}

    final timestamp = _formatDate(now);
    final filename = '${baseFileName}_$timestamp.csv';
    _file = File(p.join(folderPath, filename));
    _sink = _file!.openWrite(mode: FileMode.write);

    _currentSensorColumnLabel = sensorColumn;

    final header = ['timestamp', sensorColumn, 'sampleCount'];
    final escaped = header.map((c) => '"${c.replaceAll('"', '""')}"').join(',');
    _sink!.writeln(escaped);
    await _sink!.flush();
  }

  String _formatDate(DateTime dt) {
    String two(int v) => v.toString().padLeft(2, '0');
    return '${dt.year}${two(dt.month)}${two(dt.day)}_${two(dt.hour)}${two(dt.minute)}${two(dt.second)}';
  }
}
