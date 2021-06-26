function tests = test_Block
tests = functiontests(localfunctions);
end

function test_general_work(testCase)
tb = runtime.TopBlock();
constantSource = blocks.ConstantSource(tb,1);
delay = blocks.Delay(tb,50);
dataSink = blocks.DataSink(tb);

tb.connect(constantSource,1,delay,1);
tb.connect(delay,1,dataSink,1);

tb.run(100);
end
