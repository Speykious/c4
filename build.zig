const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(b: *std.Build) void {
    var targets = std.ArrayList(*std.Build.Step.Compile).init(b.allocator);

    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    const generalCompilerOptions = .{
        "-std=c11", "-pedantic", // C11
        "-Wall", "-Werror", // errors as warnings
        "-Wno-missing-field-initializers", // additional warnings
        "-I./include",
        "-g", // debug
        "-fsanitize=address", // ASAN
    };

    const c4_core = b.addStaticLibrary(.{
        .name = "c4-core",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4_core.addCSourceFiles(.{
        .files = &.{
            "src/core/alloc/arena.c",
            "src/core/math.c",
            "src/core/os.c",
        },
        .flags = &generalCompilerOptions,
    });
    targets.append(c4_core) catch @panic("OOM");

    const c4_app = b.addStaticLibrary(.{
        .name = "c4-app",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4_app.linkSystemLibrary("X11");
    c4_app.addCSourceFiles(.{
        .files = &.{
            "src/app/x11/x11.c",
            "src/app/x11/x11_keycode.c",
            "src/app/keycode.c",
        },
        .flags = &generalCompilerOptions,
    });
    targets.append(c4_app) catch @panic("OOM");

    const c4_audio = b.addStaticLibrary(.{
        .name = "c4-audio",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4_audio.linkSystemLibrary("asound");
    c4_audio.addCSourceFiles(.{
        .files = &.{"src/audio/audio.c"},
        .flags = &generalCompilerOptions,
    });
    targets.append(c4_audio) catch @panic("OOM");

    const c4 = b.addExecutable(.{
        .name = "c4",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4.linkSystemLibrary("asan");
    c4.linkLibrary(c4_core);
    c4.linkLibrary(c4_app);
    c4.linkLibrary(c4_audio);
    c4.addCSourceFiles(.{
        .files = &.{"src/main.c"},
        .flags = &generalCompilerOptions,
    });
    targets.append(c4) catch @panic("OOM");

    const run_step = b.addRunArtifact(c4);
    b.step("run", "Run for your fucking life.").dependOn(&run_step.step);
    b.installArtifact(c4);

    zcc.createStep(b, "cdb", .{
        .targets = targets.toOwnedSlice() catch @panic("OOM"),
    });
}
