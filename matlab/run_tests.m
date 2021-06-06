% Change the current folder to the folder of this m-file.
currPath = pwd;
if(~isdeployed)
  cd(fileparts(which(mfilename)));
end

import matlab.unittest.TestSuite
import matlab.unittest.TestRunner
import matlab.unittest.plugins.CodeCoveragePlugin
import blocks.*

suite = TestSuite.fromPackage('tests');
runner = TestRunner.withTextOutput;
% Add code coverage output for desired package(s)
runner.addPlugin(CodeCoveragePlugin.forPackage('blocks'))
result = runner.run(suite);
cd(currPath)