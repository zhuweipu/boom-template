package example

import chisel3._
import chisel3.util._
import freechips.rocketchip.config.Parameters
import freechips.rocketchip.rocket._
import freechips.rocketchip.tile.{LazyRoCC, LazyRoCCModule}

class Accumulator(implicit p: Parameters) extends LazyRoCC {
  lazy val module = new LazyRoCCModule(this) {
    val addr = Reg(UInt(64.W))
    val length = Reg(UInt(64.W))
    val value = Reg(UInt(64.W))
    val resp_rd = Reg(UInt(5.W))

    val (s_idle :: s_req :: s_resp :: s_finish :: Nil) = Enum(4)
    val state = RegInit(s_idle)

    when (io.cmd.fire()) {
      addr := io.cmd.bits.rs1
      length := io.cmd.bits.rs2
      resp_rd := io.cmd.bits.inst.rd
      value := 0.U
      state := s_req
    }

    when (io.mem.req.fire()) {
      addr := addr + 8.U
      length := length - 1.U
      state := s_resp
    }

    when (io.mem.resp.valid) {
      value := value + io.mem.resp.bits.data
      when (length === 0.U) {
        state := s_finish
      } .otherwise { state := s_req }
    }

    when (io.resp.fire()) { state := s_idle }

    io.cmd.ready := state === s_idle
    io.resp.valid := state === s_finish
    io.resp.bits.rd := resp_rd
    io.resp.bits.data := value
    io.busy := state =/= s_idle

    io.mem.req.valid := state === s_req
    io.mem.req.bits.addr := addr
    io.mem.req.bits.tag := 0.U
    io.mem.req.bits.cmd := M_XRD
    io.mem.req.bits.typ := MT_D
    io.mem.req.bits.data := 0.U
    io.mem.req.bits.phys := true.B
    io.mem.invalidate_lr := false.B
  }
}
