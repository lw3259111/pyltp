//
// Created by liu on 2017-05-12.
//

#ifndef PROJECT_TRAINSTNLSTM_H
#define PROJECT_TRAINSTNLSTM_H

#include "process/LabelModelSGDSeqTrainer.h"
#include "../config/SrlPiConfig.h"
#include "structure/SrlPiSample.h"
#include "../model/SrlPiModel.h"
#include "extractor/ConverterMultiLineFileReader.h"
#include "extractor/ConverterDataToSrlPiSample.h"
#include "extractor/ExtractorFileToWordEmb.h"


class TrainSrlPi : public LabelModelSGDSeqTrainer<SrlPiTrainConfig, SrlPiSample> {
  SrlPiTrainConfig &config;
  PiModel model;
public:
  TrainSrlPi(SrlPiTrainConfig &config)
          : LabelModelSGDSeqTrainer(config, model),
            config(config), model(config)
          {}

  void init() {
    initSample(trainSamples, config.training_data);
    initSample(devSamples, config.dev_data);

    model.registerDict(trainSamples);

    model.init(); // init model size
    model.load();
    model.initEmbedding();

  }

private:
  void initSample(vector<SrlPiSample> & samples, string file) {
    vector<DataFileName> fileName = {file};
    extractor::ConverterMultiLineFileReader fileReader;
    fileReader.init(fileName);
    fileReader.run();

    ConverterDataToSrlPiSample conv_toSample;
    conv_toSample.init(fileReader.getResult());
    conv_toSample.run();
    samples = conv_toSample.getResult();
  }
};


#endif //PROJECT_TRAINSTNLSTM_H
