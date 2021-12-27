If dicom file is compressed:

import pydicom
ds = pydicom.dcmread("IM-0003-0001.dcm")
ds.decompress()
ds.save_as('decompress.dcm')
