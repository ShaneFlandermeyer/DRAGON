function tests = test_TopBlock
tests = functiontests(localfunctions);
end

function test_constructor(testCase)
  tb = runtime.TopBlock();
  
end

function test_addBlock(testCase)
tb = runtime.TopBlock;
cs = blocks.ConstantSource(tb,1);
end