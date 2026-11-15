import os

outFile = open( "TODOs.txt", "w" )

for root, dirs, files in os.walk('.'):
  if '.svn' in dirs:
    dirs.remove('.svn')  # don't visit .svn directories
    
  for file in files:
    fn, ext = os.path.splitext( file )
    
    if ext == ".h" or ext == ".cpp":
      handle = open( root + '\\' + file, "r" )
      
      num = 0
      line = handle.readline()
      
      while line != "" :
        loc = line.find( "TODO:" )
        
        if loc != -1:
          outFile.write( file + " (" + `num` + "): " + line[loc+6:] )
        
        line = handle.readline()
        num = num + 1
        