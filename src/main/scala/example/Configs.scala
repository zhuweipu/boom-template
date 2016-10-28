package example

import cde.{Parameters, Config, CDEMatchError}
import testchipip.WithSerialAdapter
import dma.WithDma
import coreplex.WithL2Cache

class DefaultExampleConfig extends Config(
  new WithSerialAdapter ++ new rocketchip.BaseConfig)

class BaselineConfig extends Config(
  new WithL2Cache ++ new DefaultExampleConfig)

class DmaConfig extends Config(
  new WithDma ++ new BaselineConfig)
