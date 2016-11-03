package example

import util.GeneratorApp
import diplomacy.LazyModule
import rocketchip._
import testchipip._
import chisel3._
import cde.Parameters

class TestHarness(implicit val p: Parameters) extends Module {
  val io = new Bundle {
    val success = Bool(OUTPUT)
  }

  def buildTop(p: Parameters): ExampleTop = LazyModule(new ExampleTop(p))

  val dut = buildTop(p).module
  val ser = Module(new SimSerialWrapper(p(SerialInterfaceWidth)))

  val nMemChannels = dut.io.mem_axi.size
  for (axi <- dut.io.mem_axi) {
    val mem = Module(new SimDRAM)
    mem.io.axi <> axi
    mem.io.clock := clock
    mem.io.reset := reset
  }

  ser.io.serial <> dut.io.serial
  io.success := ser.io.exit || dut.io.success
}

object Generator extends GeneratorApp {
  val longName = names.topModuleProject + "." +
                 names.topModuleClass + "." +
                 names.configs
  generateFirrtl
}
