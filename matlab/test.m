clear;
tb = runtime.TopBlock();
constantSource = blocks.ConstantSource(tb,1);
dataSink = blocks.DataSink(tb);

tb.connect(constantSource,1,dataSink,1);

tb.run(100);
