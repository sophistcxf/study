import numpy as np

from scipy.spatial.transform import Rotation as R

r = R.from_quat([0, 0, np.sin(np.pi/4), np.cos(np.pi/4)])

print r.as_rotvec()
print r.as_euler('zyx', degrees=True)
