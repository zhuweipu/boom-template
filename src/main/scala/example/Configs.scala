package example

import cde.{Parameters, Config, CDEMatchError}
import testchipip.WithSerialAdapter
import dma.{WithDma, NDmaTrackers}
import coreplex.{WithL2Cache, WithPLRU}
import rocketchip.{WithNMemoryChannels, BaseConfig}
import uncore.agents.NAcquireTransactors
import groundtest.{WithGroundTest, WithMemtest}

class WithOneChannel extends WithNMemoryChannels(1)
class WithTwoChannels extends WithNMemoryChannels(2)
class WithFourChannels extends WithNMemoryChannels(4)

class WithOneTracker extends WithNDmaTrackers(1)
class WithTwoTrackers extends WithNDmaTrackers(2)
class WithFourTrackers extends WithNDmaTrackers(4)

class WithNAcquireTransactors(n: Int) extends Config(
  (pname, site, here) => pname match {
    case NAcquireTransactors => n
    case _ => throw new CDEMatchError
  })

class BaselineConfig extends Config(
  new WithNAcquireTransactors(8) ++
  new WithPLRU ++ new WithL2Cache ++ new DefaultExampleConfig)

class WithNDmaTrackers(n: Int) extends Config(
  (pname, site, here) => pname match {
    case NDmaTrackers => n
    case _ => throw new CDEMatchError
  })

class DmaConfig extends Config(
  new WithDma ++ new BaselineConfig)

class DefaultExampleConfig extends Config(
  new WithSerialAdapter ++ new rocketchip.BaseConfig)

class MemtestConfig extends Config(
  new WithMemtest ++ new WithGroundTest ++ new DefaultExampleConfig)
