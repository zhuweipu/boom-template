package example

import cde.{Parameters, Config, CDEMatchError}
import testchipip.WithSerialAdapter
import dma._
import coreplex.{WithL2Cache, WithPLRU, WithL2Capacity}
import rocketchip.{WithNMemoryChannels, BaseConfig}
import uncore.agents.NAcquireTransactors
import groundtest.{WithGroundTest, WithMemtest}
import adamacc._
import rocket._
import hwacha._
import scala.math.max
import chisel3.Module

class WithOneChannel extends WithNMemoryChannels(1)
class WithTwoChannels extends WithNMemoryChannels(2)
class WithFourChannels extends WithNMemoryChannels(4)

class WithNLanes(n: Int) extends Config(
  (pname, site, here) => pname match {
    case HwachaNLanes => n
    case _ => throw new CDEMatchError
  })

class WithOneLane extends WithNLanes(1)
class WithTwoLanes extends WithNLanes(2)
class WithFourLanes extends WithNLanes(4)

class WithOneTracker extends WithNDmaTrackers(1)
class WithTwoTrackers extends WithNDmaTrackers(2)
class WithFourTrackers extends WithNDmaTrackers(4)

class WithNAcquireTransactors(n: Int) extends Config(
  (pname, site, here) => pname match {
    case NAcquireTransactors => n
    case _ => throw new CDEMatchError
  })

class WithTwoAcquireTransactors extends WithNAcquireTransactors(2)
class WithFourAcquireTransactors extends WithNAcquireTransactors(4)
class WithEightAcquireTransactors extends WithNAcquireTransactors(8)

class BaselineConfig extends Config(
  new WithPLRU ++ new WithL2Cache ++ new DefaultExampleConfig)

class WithNDmaTrackers(n: Int) extends Config(
  (pname, site, here) => pname match {
    case NDmaTrackers => n
    case _ => throw new CDEMatchError
  })

class WithPipelineDepth(depth: Int) extends Config(
  (pname, site, here) => pname match {
    case DmaTrackerPipelineDepth => depth
    case _ => throw new CDEMatchError
  })

class WithPipelineDepth16 extends WithPipelineDepth(16)
class WithPipelineDepth64 extends WithPipelineDepth(64)
class WithPipelineDepth256 extends WithPipelineDepth(256)

class DmaConfig extends Config(
  new WithDma ++ new BaselineConfig)

class DmaTestConfig extends Config(
  new WithDma ++ new WithL2Cache ++
  new WithDmaTest ++ new WithGroundTest ++
  new DefaultExampleConfig)

class DefaultExampleConfig extends Config(
  new WithSerialAdapter ++ new rocketchip.BaseConfig)

class MemtestConfig extends Config(
  new WithMemtest ++ new WithGroundTest ++ new DefaultExampleConfig)

class HwachaConfig extends Config(
  new DefaultHwachaConfig ++ new BaselineConfig)

class AdamAccConfig extends Config(
  new WithAdamAcc ++ new BaselineConfig)

class WithAdamAccAndDma extends Config(
  (pname, site, here) => pname match {
    case BuildRoCC => Seq(
      RoccParameters(
        opcodes = OpcodeSet.custom2,
        generator = (p: Parameters) => Module(new CopyAccelerator()(p)),
        nMemChannels = site(NDmaTrackers),
        nPTWPorts = 1),
      RoccParameters(
        opcodes = OpcodeSet.custom3,
        generator = (p: Parameters) => Module(new AdamAccRoCC()(p)),
        nMemChannels = site(AdamAccMemChannels),
        nPTWPorts = 1))
    case RoccMaxTaggedMemXacts => max(
      new AdamAccMemParams(site).roccXacts,
      3 * site(NDmaTrackerMemXacts))
    case _ => throw new CDEMatchError
  })

class AdamAccDmaConfig extends Config(
  new WithAdamAccAndDma ++ new WithAdamAcc ++ new WithDma ++
  new BaselineConfig)

class NoUnitAdamAccConfig extends Config(
  new WithNoUnits ++ new AdamAccConfig)

class PartitionerAdamAccConfig extends Config(
  new WithPartitioner ++ new NoUnitAdamAccConfig)

class SorterAdamAccConfig extends Config(
  new WithSorter ++ new NoUnitAdamAccConfig)

class IRAdamAccConfig extends Config(
  new WithIR ++ new NoUnitAdamAccConfig)
