APPNAME = 'grasp'
VERSION = '1.0.0'

srcdir = '.'
blddir = 'build'

def options(opt):
    opt.load("compiler_cxx")
    opt.recurse("libs")

def configure(conf):
    conf.load("compiler_cxx")
    conf.recurse("libs")

def build(bld):
    bld.install_files('${PREFIX}/include', bld.path.ant_glob("**/*.hpp"), 
            relative_trick = True)
    bld.recurse("libs")

def shutdown(ctx):
    # 最後に呼ばれる
	pass
