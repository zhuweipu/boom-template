package example

import chisel3._
import chisel3.internal.sourceinfo.{SourceInfo, SourceLine}
import freechips.rocketchip.util.property._
import freechips.rocketchip.config.{Parameters, Config}
import freechips.rocketchip.coreplex.{WithRoccExample, WithNMemoryChannels, WithNBigCores, WithRV32}
import freechips.rocketchip.diplomacy.{LazyModule, ValName}
import freechips.rocketchip.devices.tilelink.BootROMParams
import freechips.rocketchip.tile.XLen
import freechips.rocketchip.coreplex.RocketTilesKey
import testchipip._

class WithBootROM extends Config((site, here, up) => {
  case BootROMParams => BootROMParams(
    contentFileName = s"./bootrom/bootrom.rv${site(XLen)}.img")
})

object ConfigValName {
  implicit val valName = ValName("TestHarness")
}
import ConfigValName._

class WithExampleTop extends Config((site, here, up) => {
  case BuildTop => (clock: Clock, reset: Bool, p: Parameters) => {
    Module(LazyModule(new ExampleTop()(p)).module)
  }
})

class WithPWM extends Config((site, here, up) => {
  case BuildTop => (clock: Clock, reset: Bool, p: Parameters) =>
    Module(LazyModule(new ExampleTopWithPWM()(p)).module)
})

class WithBlockDeviceModel extends Config((site, here, up) => {
  case BuildTop => (clock: Clock, reset: Bool, p: Parameters) => {
    val top = Module(LazyModule(new ExampleTopWithBlockDevice()(p)).module)
    top.connectBlockDeviceModel()
    top
  }
})

class WithSimBlockDevice extends Config((site, here, up) => {
  case BuildTop => (clock: Clock, reset: Bool, p: Parameters) => {
    val top = Module(LazyModule(new ExampleTopWithBlockDevice()(p)).module)
    top.connectSimBlockDevice(clock, reset)
    top
  }
})

class WithICache extends Config((site, here, up) => {
  case RocketTilesKey => up(RocketTilesKey) map (tile => tile.copy(
    icache = tile.icache map (_.copy(
      nSets = 64,
      nWays = 4,
      rowBits = 128
    ))
  ))
})

class PrintfPropertyLibrary extends BasePropertyLibrary {
  def generateProperty(prop_param: BasePropertyParameters)(implicit sourceInfo: SourceInfo) = {
    prop_param match {
      case CoverPropertyParameters(cond, label, message) =>
        printf(p"COVER:$cond:$label:$message\n")
      case _ => ???
    }
  }
}

class WithCoverage extends Config((site, here, up) => {
  case PropertyLibrary => new PrintfPropertyLibrary
})

class BaseExampleConfig extends Config(
  new WithBootROM ++
  new WithICache ++
  new WithCoverage ++
  new freechips.rocketchip.system.DefaultConfig)

class DefaultExampleConfig extends Config(
  new WithExampleTop ++ new BaseExampleConfig)

class RoccExampleConfig extends Config(
  new WithRoccExample ++ new DefaultExampleConfig)

class PWMConfig extends Config(new WithPWM ++ new BaseExampleConfig)

class SimBlockDeviceConfig extends Config(
  new WithBlockDevice ++ new WithSimBlockDevice ++ new BaseExampleConfig)

class BlockDeviceModelConfig extends Config(
  new WithBlockDevice ++ new WithBlockDeviceModel ++ new BaseExampleConfig)

class WithTwoTrackers extends WithNBlockDeviceTrackers(2)
class WithFourTrackers extends WithNBlockDeviceTrackers(4)

class WithTwoMemChannels extends WithNMemoryChannels(2)
class WithFourMemChannels extends WithNMemoryChannels(4)

class DualCoreConfig extends Config(
  // Core gets tacked onto existing list
  new WithNBigCores(1) ++ new DefaultExampleConfig)

class RV32ExampleConfig extends Config(
  new WithRV32 ++ new DefaultExampleConfig)
