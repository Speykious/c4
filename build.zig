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

    const cingle_app = b.addStaticLibrary(.{
        .name = "cingle-audio",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    cingle_app.force_pic = true;
    cingle_app.addCSourceFiles(&.{
        "src/app/x11.c",
    }, &generalCompilerOptions);
    targets.append(cingle_app) catch @panic("OOM");

    const cingle_audio = b.addStaticLibrary(.{
        .name = "cingle-audio",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    cingle_audio.force_pic = true;
    cingle_audio.addCSourceFiles(&.{
        "src/audio/audio.c",
    }, &generalCompilerOptions);
    targets.append(cingle_audio) catch @panic("OOM");

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
    b.installArtifact(cingle);

    zcc.createStep(b, "cdb", targets.toOwnedSlice() catch @panic("OOM"));
}
