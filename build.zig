const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(b: *std.build.Builder) void {
    var targets = std.ArrayList(*std.Build.CompileStep).init(b.allocator);

    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    const generalCompilerOptions = .{
        "-std=c11",
        "-pedantic",
        "-Wall",
        "-Werror",
        "-W",
        "-Wno-missing-field-initializers",
    };

    const cingle_core = b.addStaticLibrary(.{
        .name = "cingle-core",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    cingle_core.force_pic = true;
    cingle_core.addCSourceFiles(&.{
        "src/core/alloc/arena.c",
        "src/core/math.c",
        "src/core/os.c",
    }, &generalCompilerOptions);
    targets.append(cingle_core) catch @panic("OOM");

    const cingle = b.addExecutable(.{
        .name = "cingle",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    cingle.linkLibrary(cingle_core);
    cingle.addCSourceFiles(&.{
        "src/main.c",
    }, &generalCompilerOptions);
    targets.append(cingle) catch @panic("OOM");

    const run_step = b.addRunArtifact(cingle);
    b.step("run", "Run for your fucking life.").dependOn(&run_step.step);

    zcc.createStep(b, "cdb", targets.toOwnedSlice() catch @panic("OOM"));
}
