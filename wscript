APPNAME = 'savage'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

recurse_dirs = ['libs']

def options(opt):
    opt.load("compiler_cxx")

def configure(conf):
    conf.load("compiler_cxx")

def build(bld):
    bld.install_files('${PREFIX}/include', bld.path.ant_glob("savage/**/*.hpp"), 
            relative_trick = True)
    bld.install_files('${PREFIX}/include', bld.path.ant_glob("savage/3rdparty/tiny_obj_loader.*"), 
            relative_trick = True)
    bld.recurse(recurse_dirs)

def shutdown(ctx):
    pass
