import os

nwilliam = open( "nathanwilliams.txt", "w" )
ctallman = open( "christallman.txt", "w" )
pmera = open( "patrickmera.txt", "w" )
#unknown = open( "unknown.txt", "w" )

for root, dirs, files in os.walk('.'):
  if '.svn' in dirs:
    dirs.remove('.svn')  # don't visit .svn directories
    
  for file in files:
    fn, ext = os.path.splitext( file )
    
    if ext == ".h" or ext == ".cpp" :
      handle = open( root + '\\' + file, "r" )
      
      num = 0
      line = handle.readline()
      line = handle.readline()
      line = handle.readline()
      
      if line.find( "Nathan" ) != -1:
        nwilliam.write( file + "\n" )
      elif line.find( "Chris" ) != -1:
        ctallman.write( file + "\n" )
      elif line.find( "Patrick" ) != -1:
        pmera.write( file  + "\n" )
      #else:
      #  unknown.write( file + "\n" )
      
