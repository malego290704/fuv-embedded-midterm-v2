#ifndef _AI_ENGINE_H
#define _AI_ENGINE_H

#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "smartLockModel.h"

#include "configuration.h"

constexpr uint32_t _kTensorArenaSize = g_model_len * 4;

constexpr uint32_t aiInputSize = 24 * 24;
constexpr uint32_t aiOutputSize = 2;
char aiOutputLabels[MAX_USER_COUNT][MAX_USER_ID_LENGTH] = {"an", "minh"};
char aiOutputNoLabel[MAX_USER_ID_LENGTH] = "guest";

typedef struct AIEngine {
  TfLiteTensor* input;
  TfLiteTensor* output;
  const uint32_t kTensorArenaSize = _kTensorArenaSize;
  uint8_t tensorArena[_kTensorArenaSize];
  const tflite::Model* model;
  tflite::MicroInterpreter* intepreter;
  tflite::MicroMutableOpResolver<10> resolver;
  tflite::MicroErrorReporter microErrorReporter;
  tflite::ErrorReporter* errorReporter = nullptr;
  void init() {
    errorReporter = &microErrorReporter;
    this->model = tflite::GetModel(g_model);
    if (!model) {
      errorReporter->Report("Failed to get model from g_model array");
      return;
    }
    this->resolver.AddFullyConnected();
    this->resolver.AddConv2D();
    this->resolver.AddMaxPool2D();
    this->resolver.AddSoftmax();
    this->resolver.AddReshape();
    this->resolver.AddRelu();
    this->resolver.AddStridedSlice();
    this->resolver.AddShape();
    this->resolver.AddPack();
    this->intepreter = new tflite::MicroInterpreter(this->model, this->resolver, this->tensorArena, this->kTensorArenaSize, this->errorReporter);
    if (this->intepreter->AllocateTensors() != kTfLiteOk) {
      errorReporter->Report("AllocateTensors() failed.");
      return;
    }
    this->input = this->intepreter->input(0);
    this->output = this->intepreter->output(0);
  }
  char* predict(float input1D[]) {
    for (int i = 0; i < aiInputSize; i++) {
      this->input->data.f[i] = input1D[i];
    }
    if (this->intepreter->Invoke() != kTfLiteOk) {
      return aiOutputNoLabel;
    }
    float max_prob = 0.0f;
    int predicted_class_index = -1;
    for (int i = 0; i < aiOutputSize; i++) {
      float current_prob = output->data.f[i];
      if (current_prob > max_prob) {
        max_prob = current_prob;
        predicted_class_index = i;
      }
    }
    if (max_prob > 0.85f) {
      return aiOutputLabels[predicted_class_index];
    } else {
      return aiOutputNoLabel;
    }
  }
} AIEngine;

#endif