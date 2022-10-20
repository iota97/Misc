import numpy as np
import quaternion
import matplotlib.pyplot as plt

def quatRot(theta, w, p):
    w = w/np.sqrt(np.sum(w**2))*np.sin(theta/2)
    q = np.quaternion(np.cos(theta/2), w[0], w[1], w[2])
    p = np.quaternion(0, p[0], p[1], p[2])
    p = np.multiply(np.multiply(q, p), np.conjugate(q))
    return np.array([p.x, p.y, p.z])

def affRot(theta, w, p):
    w = w/np.sqrt(np.sum(w**2))
    c = np.cross(w, np.array([0, 0, 1]))
    if (np.sum(c**2) == 0):
        R = np.array([[np.cos(theta), -np.sin(theta), 0],
                    [np.sin(theta), np.cos(theta), 0],
                    [0, 0, 1]])
        p = np.matmul(R, p)
    else:
        c = c/np.sqrt(np.sum(c**2))

        A = np.array([[c[0], np.cross(w, c)[0], w[0]],
                    [c[1], np.cross(w, c)[1], w[1]],
                    [c[2], np.cross(w, c)[2], w[2]]])

        R = np.array([[np.cos(theta), -np.sin(theta), 0],
                    [np.sin(theta), np.cos(theta), 0],
                    [0, 0, 1]])
        p = np.matmul(A, np.matmul(R, np.matmul(A.transpose(), p)))
    return p

def err(theta, w, p):
    quat = quatRot(theta, w, p)
    aff = affRot(theta, w, p)
    err = np.sum((quat-aff)**2)/np.sum((quat)**2)
    return [np.dot(w/np.sqrt(np.sum(w**2)), np.array([0, 0, 1])), err]

def instance():
    theta = np.random.random()*np.pi*2
    w = np.array([0, 0, 0])
    while (np.sum(w**2) < 0.5):
        w = np.array([1.0-np.random.random()*2, 1.0-np.random.random()*2, 1.0-np.random.random()*2])
    p = np.array([(1.0-np.random.random()*2)*500000, (1.0-np.random.random()*2)*500000, (1.0-np.random.random()*2)*500000])

    return(err(theta, w, p))

def getResult(r):
    d = {}
    for _ in range(r):
        r = instance()
        if r[0] in d:
            d[r[0]].append(r[1])
        else:
            d[r[0]] = np.array([r[1]])
    return d

def plot(r):
    d = getResult(r)
    x = []
    y = []
    for i in d:
        x.append(i)
    x.sort()

    for i in x:
        y.append(np.mean(d[i]))

    plt.xlabel('Cosine')
    plt.ylabel('Relative error')
    plt.title('Quaternion vs Affinity')
    plt.plot(x, y, 'bo', ms=1)
    plt.show()

plot(100000)
