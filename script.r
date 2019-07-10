library(sets)
sets_options("universe", seq(1, 80, 0.5))

args <- commandArgs(TRUE)

## Input Simulation parameters

temperatura<-as.integer(args[1])   
humidade<-as.integer(args[2])    
luminosidade<-as.integer(args[3])

variables = set(
  IDADE = fuzzy_variable(
    JOVEM = fuzzy_triangular(corners = c(14, 18, 24)),
    ADULTO = fuzzy_triangular(corners = c(22, 37, 50)),
    VELHO = fuzzy_trapezoid(corners = c(45, 64, 80, 100))
  ),

  EXPERIENCIA = fuzzy_variable(
    NOVATO = fuzzy_triangular(corners = c(0, 0, 5)),
    EXPERIENTE = fuzzy_trapezoid(corners = c(3, 10, 20, 30)),
    MUITO_EXPERIENTE = fuzzy_trapezoid(corners = c(25, 49, 60, 100))
  ),

  EDUCACAO = fuzzy_variable(
    FUNDAMENTAL = fuzzy_trapezoid(corners = c(0, 0, 6, 10)),
    MEDIO = fuzzy_triangular(corners = c(8, 13, 15)),
    SUPERIOR = fuzzy_trapezoid(corners = c(14, 18, 50, 50))
  ),
  
  SALARIO = fuzzy_variable(
    BAIXO = fuzzy_trapezoid(corners = c(0, 0, 3, 8)),
    MEDIO = fuzzy_trapezoid(corners = c(7, 9, 18, 25)),
    ALTO  = fuzzy_trapezoid(corners = c(20, 45, 60, 100))
  )
)

#Regras
regras = set(
  fuzzy_rule( EXPERIENCIA %is% EXPERIENTE , SALARIO %is% MEDIO),
  fuzzy_rule( EXPERIENCIA %is% MUITO_EXPERIENTE || EDUCACAO %is% SUPERIOR,  SALARIO% is% ALTO),
  fuzzy_rule( EXPERIENCIA %is% NOVATO || IDADE %is% JOVEM, SALARIO %is% BAIXO),
  fuzzy_rule( EXPERIENCIA %is% EXPERIENTE && EDUCACAO %is% MEDIO, SALARIO %is% ALTO),
  fuzzy_rule( EXPERIENCIA %is% EXPERIENTE && IDADE %is% JOVEM, SALARIO %is% MEDIO),
  fuzzy_rule( EDUCACAO %is% FUNDAMENTAL, SALARIO %is% BAIXO),
  fuzzy_rule( EDUCACAO %is% MEDIO &&  IDADE %is% ADULTO, SALARIO %is% MEDIO),
  fuzzy_rule( EDUCACAO %is% SUPERIOR && EXPERIENCIA %is% NOVATO, SALARIO %is% MEDIO),
  fuzzy_rule( IDADE %is% JOVEM && EDUCACAO %is% SUPERIOR, SALARIO %is% MEDIO),
  fuzzy_rule( IDADE %is% ADULTO && EXPERIENCIA %is% NOVATO, SALARIO %is% BAIXO),
  fuzzy_rule( IDADE %is% VELHO && EXPERIENCIA %is% NOVATO, SALARIO %is% BAIXO),
  fuzzy_rule( IDADE %is% VELHO && EXPERIENCIA %is% EXPERIENTE, SALARIO %is% MEDIO)
)

model = fuzzy_system(variables, regras)

ex <- fuzzy_inference(model, list(IDADE=temperatura, EXPERIENCIA=luminosidade, 
                                  EDUCACAO=humidade))

data = gset_defuzzify(ex, "centroid") #defusicacao 

write.table(data, file = "saida.txt", sep = "\t", row.names = FALSE)
