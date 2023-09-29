import controlP5.*;
import processing.serial.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.*;

Serial port;
Chart myChart1;
Chart myChart2;
Chart myChart3;
ControlP5 cp5;
PFont font;
int nl = 10;
String received = null;
String[] readings;
int stop_flag = 0;
int start_flag = 0;
PrintWriter save_data;

void setup() {
  size(800, 450);

  port = new Serial(this, "COM10", 115200);

  cp5 = new ControlP5(this);
  font = createFont("calibri light bold", 20);

  cp5.addButton("Start")
    .setPosition(80, 120)
    .setSize(120, 70)
    .setFont(font);

  cp5.addButton("Stop")
    .setPosition(80, 220)
    .setSize(120, 70)
    .setFont(font);

  cp5.addButton("Save")
    .setPosition(80, 320)
    .setSize(120, 70)
    .setFont(font);

  myChart1 = cp5.addChart("pulse waveform 1")
    .setPosition(260, 10)
    .setSize(500, 125)
    .setView(Chart.LINE)
    .setRange(65000, 85000)
    .setStrokeWeight(1.5);

  myChart1.addDataSet("incoming");
  myChart1.setData("incoming", new float[100]);

  myChart2 = cp5.addChart("pulse waveform 2")
    .setPosition(260, 155)
    .setSize(500, 125)
    .setView(Chart.LINE)
    .setRange(120000, 130000);

  myChart2.addDataSet("incoming");
  myChart2.setData("incoming", new float[100]);

  myChart3 = cp5.addChart("pulse waveform 3")
    .setPosition(260, 300)
    .setSize(500, 125)
    .setView(Chart.LINE)
    .setRange(65000, 85000);

  myChart3.addDataSet("incoming");
  myChart3.setData("incoming", new float[100]);

  save_data = createWriter("sensordata.txt" + day() + "-" + month() + "-" + year() + ":" + hour() + "." + minute());
}

void draw() {
  background(0, 0, 0);

  fill(0, 255, 0);
  textFont(font);
  text("PULSE WAVEFORM", 60, 80);

  received = port.readStringUntil(nl);
  if (received != null && start_flag == 1 && stop_flag == 0) {
    readings = received.split(",", 3);
    println(readings[0], readings[1], readings[2]);
    save_data.println(hour() + "." + "0" + minute() + "." + second() + "." + millis() + " : " + readings[0] + "," + readings[1] + "," + readings[2]);
    myChart1.push("incoming", float(readings[0]));
    myChart2.push("incoming", float(readings[1]));
    myChart3.push("incoming", float(readings[2]));
  }
}

void Start() {
  start_flag = 1;
  stop_flag = 0;
}

void Stop() {
  stop_flag = 1;
  start_flag = 0;
}

void Save() {
  save_data.flush();
  save_data.close();
  save_data = createWriter("sensordata.txt" + day() + "/" + month() + "/" + year() + ":" + hour() + "." + minute());
}
