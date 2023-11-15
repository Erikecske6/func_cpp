# Tesztek megoldása

# Teszt #1

**_1._** Jelölje meg az alábbi _λ-kifejezés_ redukáltját!

_**(λx.x)(λs.ss)**_
 
- [ ] **λx.x**
- [x] **λs.ss**
- [ ] **λx.(λs.ss)**
- [ ] **xs**

  
**_2._** A λ-kalkulusban minden függvénynek csak egy változója lehet. A többváltozós függvények átalakíthatóak egyváltozós függvények applikációinak a sorozatává. Hogy nevezzük ezt a műveletet?

***körrizés***

**_3._** A _**λxyz.xyz**_ _λ-kifejezés_ szintaktikailag azonos a … *λ-kifejezés*sel.

- [x] **λx.(λy.(λz.((xy)z)))**
- [ ] **λx.(λy.(λz.(x(yz)))**
- [ ] **λx.(λy.(λz.x)y)z**
- [ ] **λy.(λz.zy)**

**_4._** Döntse el, hogy az alábbi kifejezésben az **x** változó szabad-e vagy sem!

**(λy.(xz)λy.y)λy.(zy)**

**igen/~~nem~~**

**_5._** Hogyan írná át λ-kifejezéssé az alábbi C++ függvényt?
```
[](auto x) {  
    return [=](auto y) {  
        return [=](auto z) {  
            return x(y(z));  
        }  
    }  
}
```

- [ ] **λyxz.xyz**
- [x] **λx.λy.λz.x(yz)**
- [ ] **λxyz.(xy)z**
- [ ] **λy.(λx.λz.xy)z**

Segítség:
```
xy           --->  x(y)
xyz = (xy)z  ---> (x(y))(z)
x(yz)        ---> x(y(z))
```

# Teszt #2

**_1._** A _**(λxyz.zxy)uv(λyz.y)**_ _λ-kifejezés_ _β-redukcióját_ a _**(λyz.y)**_ behelyettesítésével kell kezdeni.

- [ ] igen
- [x] nem

Segítség:
```
(λxyz.zxy)uv(λyz.y) ≡ ((((λxyz.zxy)u)v)(λyz.y))
```

**_2._** Az órán bemutatott elsőrendű típusos kalkulusunkban a megszokott β-redukció az egyetlen evolúciós szabály. Hogyan definiálná a hozzá tartozó típusszabályt? Az alábbi következtetésben adja meg a Θ értét.

***B***

**_3._** Igaz-e az alábbi állítás?

Az előadáson bemutatott F1 típusrendszerben az E:A jól típusozott kifejezésnek nincs végtelen β-redukciós sora.

- [x] igen
- [ ] nem

**_4._**  Az alábbiak közül melyik nem redukálási stratégia?

- [ ] call-by-name
- [ ] érték szerinti
- [x] λ-szerinti

**_5._** Adva van a _**λz.y(λx.xy)z**_ _λ-kifejezés_. Rajzolja fel a kifejezés gráfját!

Segítség:
```
λz.y(λx.xy)z ≡ λz.((y(λx.xy))z)
```
