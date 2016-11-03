package example

import cde.{Parameters, Config, CDEMatchError}
import testchipip.WithSerialAdapter
import groundtest.{WithGroundTest, WithMemtest}

class DefaultExampleConfig extends Config(
  new WithSerialAdapter ++ new rocketchip.BaseConfig)

class MemtestConfig extends Config(
  new WithMemtest ++ new WithGroundTest ++ new DefaultExampleConfig)
